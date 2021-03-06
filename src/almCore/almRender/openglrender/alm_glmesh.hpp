#ifndef _ALM_STANDART_MESH_HPP_
#define _ALM_STANDART_MESH_HPP_

#include "../src/almCore/almRender/interface/alm_imesh.hpp"

namespace alme
{

class AlmMesh : public IAlmMesh
{
public:
	AlmMesh();
	~AlmMesh();

	uint32_t GetId() const override;
	uint32_t RenderId() const override;

	void SetIndexes(const std::vector<uint32_t> &indeces) override;
	void SetVertexes(const std::vector<kmu::vec3> &vertexes) override;

	void SetIndexes(std::vector<uint32_t> && indeces) override;
	void SetVertexes(std::vector<kmu::vec3> && vertexes) override;

	void SetUW(std::vector<kmu::vec2> && uw) override;
	void SetUW(const std::vector<kmu::vec2> &uw) override;

	void SetNormals(std::vector<kmu::vec3> && normals) override;
	void SetNormals(const std::vector<kmu::vec3> &normals) override;

	std::vector<kmu::vec2> GetUW() const override;
	std::vector<uint32_t> GetIndexes() const override;
	std::vector<kmu::vec3> GetNormals() const override;
	std::vector<kmu::vec3> GetVertexes() const override;

	uint32_t GetSizeInBytes() const override;
	bool IsSubmited() const override;

	void LoadFromFile(const std::string &filepath) override;

	void Submit() override;
	void Release() override;

	const sAlmBoundingBox3D & GetBoundingBox() const override;

private:
	void BuildBoundingBox();
	void BuildBoundingBox(const kmu::vec3 & pos, const kmu::vec3 & max, const kmu::vec3 & min);

private:
	uint32_t m_vaoId;
	uint32_t m_vrtBufferId;
	uint32_t m_idxBufferId;
	std::vector<kmu::vec2> m_uw;
	std::vector<uint32_t> m_indeces;
	std::vector<kmu::vec3> m_normals;
	std::vector<kmu::vec3> m_vertexes;
	sAlmBoundingBox3D m_boundingBox;
};

}
#endif // !_ALM_STANDART_MESH_HPP_