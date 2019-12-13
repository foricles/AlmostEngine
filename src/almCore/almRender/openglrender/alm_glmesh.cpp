#include "alm_glmesh.hpp"
#include <glew/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"

using namespace alme;

inline kmu::vec3 vmax(const kmu::vec3 &lhv, const kmu::vec3 &rhv)
{
	return std::move(kmu::vec3(std::max(lhv.x, rhv.x), std::max(lhv.y, rhv.y), std::max(lhv.z, rhv.z)));
}

inline kmu::vec3 vmin(const kmu::vec3 &lhv, const kmu::vec3 &rhv)
{
	return std::move(kmu::vec3(std::min(lhv.x, rhv.x), std::min(lhv.y, rhv.y), std::min(lhv.z, rhv.z)));
}

inline kmu::vec3 vabs(const kmu::vec3 &v)
{
	return std::move(kmu::vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z)));
}


AlmMesh::AlmMesh()
	: m_vaoId(0)
	, m_vrtBufferId(0)
	, m_idxBufferId(0)
	, m_boundingBox()
{
}

AlmMesh::~AlmMesh()
{
	Release();
}

uint32_t AlmMesh::GetId() const
{
	return uint32_t();
}

uint32_t AlmMesh::RenderId() const
{
	return m_vaoId;
}

void AlmMesh::SetIndexes(const std::vector<uint32_t>& indeces)
{
	m_indeces = indeces;
}

void AlmMesh::SetVertexes(const std::vector<kmu::vec3>& vertexes)
{
	m_vertexes = vertexes;
	BuildBoundingBox();
}

void AlmMesh::SetIndexes(std::vector<uint32_t>&& indeces)
{
	m_indeces = std::move(indeces);
}

void AlmMesh::SetVertexes(std::vector<kmu::vec3>&& vertexes)
{
	m_vertexes = std::move(vertexes);
	BuildBoundingBox();
}

void AlmMesh::SetUW(std::vector<kmu::vec2>&& uw)
{
	m_uw = std::move(uw);
}

void AlmMesh::SetUW(const std::vector<kmu::vec2>& uw)
{
	m_uw = uw;
}

void AlmMesh::SetNormals(std::vector<kmu::vec3>&& normals)
{
	m_normals = std::move(normals);
}

void AlmMesh::SetNormals(const std::vector<kmu::vec3>& normals)
{
	m_normals = normals;
}

std::vector<kmu::vec2> AlmMesh::GetUW() const
{
	return m_uw;
}

std::vector<uint32_t> AlmMesh::GetIndexes() const
{
	return m_indeces;
}

std::vector<kmu::vec3> AlmMesh::GetNormals() const
{
	return m_normals;
}

std::vector<kmu::vec3> AlmMesh::GetVertexes() const
{
	return m_vertexes;
}

uint32_t AlmMesh::GetSizeInBytes() const
{
	return m_uw.size() * sizeof(kmu::vec2)
		+ m_indeces.size()*sizeof(uint32_t)
		+ m_normals.size()*sizeof(kmu::vec3)
		+ m_vertexes.size()*sizeof(kmu::vec3);
}

bool AlmMesh::IsSubmited() const
{
	return m_vaoId > 0;
}

void AlmMesh::LoadFromFile(const std::string & filepath)
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

	kmu::vec3 max;
	kmu::vec3 min;
	kmu::vec3 midle;

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

		max.x = std::max(max.x, pPos->x);
		max.y = std::max(max.y, pPos->y);
		max.z = std::max(max.z, pPos->z);

		min.x = std::min(min.x, pPos->x);
		min.y = std::min(min.y, pPos->y);
		min.z = std::min(min.z, pPos->z);

		midle.x += pPos->x;
		midle.y += pPos->y;
		midle.z += pPos->z;
	}

	//Load indexes
	m_indeces.reserve(paiMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		for (unsigned int j(0); j < face.mNumIndices; ++i)
			m_indeces.push_back(face.mIndices[j]);
	}

	midle /= paiMesh->mNumVertices;
	BuildBoundingBox(midle, max, min);
}

void AlmMesh::Submit()
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

void AlmMesh::Release()
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

const sAlmBoundingBox3D & alme::AlmMesh::GetBoundingBox() const
{
	return m_boundingBox;
}

void AlmMesh::BuildBoundingBox()
{
	kmu::vec3 max;
	kmu::vec3 min;
	kmu::vec3 midle;
	for (auto & vert : m_vertexes)
	{
		max = vmax(max, vert);
		min = vmin(min, vert);
		midle += vert;
	}
	midle /= static_cast<float>(m_vertexes.size());
	BuildBoundingBox(midle, max, min);
}

void AlmMesh::BuildBoundingBox(const kmu::vec3 & pos, const kmu::vec3 & max, const kmu::vec3 & min)
{
	kmu::vec3 dist = vabs(min - max) / 2.0f;
	kmu::vec3 up = pos + kmu::vec3(0, dist.y, 0);
	m_boundingBox.uP1 = up + kmu::vec3(dist.x, 0, 0);
	m_boundingBox.uP2 = up + kmu::vec3(0, 0, dist.y);
	m_boundingBox.uP3 = up - kmu::vec3(0, 0, dist.y);
	m_boundingBox.uP4 = up - kmu::vec3(dist.x, 0, 0);

	for (int i = 0; i < 4; ++i)
		m_boundingBox.n[i + 4] = m_boundingBox.n[i] - kmu::vec3(0, dist.y * 2.0f ,0);
}
