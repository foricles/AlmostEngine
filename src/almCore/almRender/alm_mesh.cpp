#include "alm_mesh.hpp"
#include <glew/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"

using namespace alme;

AlmMeash::AlmMeash()
	: m_vaoId(0)
	, m_vrtBufferId(0)
	, m_idxBufferId(0)
{
}

AlmMeash::~AlmMeash()
{
	Release();
}

uint32_t AlmMeash::GetId() const
{
	return uint32_t();
}

uint32_t AlmMeash::RenderId() const
{
	return m_vaoId;
}

void AlmMeash::SetIndexes(const std::vector<uint32_t>& indeces)
{
	m_indeces = indeces;
}

void AlmMeash::SetVertexes(const std::vector<kmu::vec3>& vertexes)
{
	m_vertexes = vertexes;
}

void AlmMeash::SetIndexes(std::vector<uint32_t>&& indeces)
{
	m_indeces = std::move(indeces);
}

void AlmMeash::SetVertexes(std::vector<kmu::vec3>&& vertexes)
{
	m_vertexes = std::move(vertexes);
}

void AlmMeash::SetUW(std::vector<kmu::vec2>&& uw)
{
	m_uw = std::move(uw);
}

void AlmMeash::SetUW(const std::vector<kmu::vec2>& uw)
{
	m_uw = uw;
}

void AlmMeash::SetNormals(std::vector<kmu::vec3>&& normals)
{
	m_normals = std::move(normals);
}

void AlmMeash::SetNormals(const std::vector<kmu::vec3>& normals)
{
	m_normals = normals;
}

std::vector<kmu::vec2> AlmMeash::GetUW() const
{
	return m_uw;
}

std::vector<uint32_t> AlmMeash::GetIndexes() const
{
	return m_indeces;
}

std::vector<kmu::vec3> AlmMeash::GetNormals() const
{
	return m_normals;
}

std::vector<kmu::vec3> AlmMeash::GetVertexes() const
{
	return m_vertexes;
}

uint32_t AlmMeash::GetSizeInBytes() const
{
	return m_uw.size() * sizeof(kmu::vec2)
		+ m_indeces.size()*sizeof(uint32_t)
		+ m_normals.size()*sizeof(kmu::vec3)
		+ m_vertexes.size()*sizeof(kmu::vec3);
}

bool AlmMeash::IsSubmited() const
{
	return m_vaoId > 0;
}

void AlmMeash::LoadFromFile(const std::string & filepath)
{
	io::AlmFile model(filepath);
	if (!model.Exist())
	{
		ALM_LOG_ERROR("Model file not exist", filepath);
		return;
	}

	Assimp::Importer importer;
	std::vector<uint8_t> modelData = model.Load().asBin();

	const aiScene *scene = importer.ReadFileFromMemory(modelData.data(), modelData.size(), aiProcessPreset_TargetRealtime_Fast);

	if (scene == nullptr || (scene && scene->mNumMeshes <= 0))
	{
		ALM_LOG_ERROR("Error in parse model", filepath);
		return;
	}

	const aiMesh* paiMesh = scene->mMeshes[0];
	m_uw.reserve(paiMesh->mNumVertices);
	m_normals.reserve(paiMesh->mNumVertices);
	m_vertexes.reserve(paiMesh->mNumVertices);

	//Load vertexes
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D *pPos = &(paiMesh->mVertices[i]);
		const aiVector3D *pNor = &(paiMesh->mNormals[i]);
		const aiVector3D *pBnor = &(paiMesh->mBitangents[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &aiVector3D(0, 0, 0);

		m_uw.emplace_back(pTexCoord->x, pTexCoord->y);
		m_normals.emplace_back(pNor->x, pNor->y, pNor->z);
		m_vertexes.emplace_back(pPos->x, pPos->y, pPos->z);
	}

	//Load indexes
	m_indeces.reserve(paiMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		for (unsigned int j(0); j < face.mNumIndices; ++i)
			m_indeces.push_back(face.mIndices[j]);
	}
}

void AlmMeash::Submit()
{
	if (m_vaoId == 0)
	{
		glGenVertexArrays(1, &m_vaoId);
		glGenBuffers(1, &m_vrtBufferId);
		glGenBuffers(1, &m_idxBufferId);
	}
	else
	{
		Release();
	}

	const uint32_t u_size = m_uw.size() * sizeof(kmu::vec2);
	const uint32_t n_size = m_normals.size() * sizeof(kmu::vec3);
	const uint32_t v_size = m_vertexes.size() * sizeof(kmu::vec3);

	glBindVertexArray(m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vrtBufferId);
	glBufferData(GL_ARRAY_BUFFER, u_size + n_size + v_size, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0,	v_size, m_vertexes.data());
	glBufferSubData(GL_ARRAY_BUFFER, v_size, n_size, m_normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, v_size + n_size, u_size, m_uw.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indeces.size() * sizeof(uint32_t), m_indeces.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(kmu::vec3), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(kmu::vec3), (void*)(v_size));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(kmu::vec2), (void*)(v_size + n_size));

	glBindVertexArray(0);
}

void AlmMeash::Release()
{
	if (m_vaoId <= 0)
		return;

	GLint current_vao = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
	if (current_vao == static_cast<GLint>(m_vaoId))
		glBindVertexArray(0);

	glDeleteVertexArrays(1, &m_vaoId);
	glDeleteBuffers(1, &m_vrtBufferId);
	glDeleteBuffers(1, &m_idxBufferId);

	m_vaoId = m_vrtBufferId = m_idxBufferId = 0;
}
