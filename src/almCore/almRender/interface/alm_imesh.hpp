#ifndef _ALM_MESH_INTERFACE_HPP_
#define _ALM_MESH_INTERFACE_HPP_

#include <string>
#include <vector>
#include "../src/almCore/almMath/vec3.hpp"
#include "../src/almCore/almMath/vec2.hpp"

namespace alme
{

struct sAlmBoundingBox3D
{
	union
	{
		struct { kmu::vec3 uP1, uP2, uP3, uP4, dP1, dP2, dP3, dP4; };
		struct { kmu::vec3 n[8]; };
	};

	sAlmBoundingBox3D() { n[0] = n[1] = n[2] = n[3] = n[4] = n[5] = n[6] = n[7] = kmu::vec3(); }
	~sAlmBoundingBox3D() {}
};

class IAlmMesh
{
public:
	IAlmMesh() {};
	virtual ~IAlmMesh() {};

	virtual uint32_t				GetId() const = 0;
	virtual uint32_t				RenderId() const = 0;

	virtual void					SetIndexes(std::vector<uint32_t> && indeces) = 0;
	virtual void					SetIndexes(const std::vector<uint32_t> &indeces) = 0;

	virtual void					SetVertexes(std::vector<kmu::vec3> && vertexes) = 0;
	virtual void					SetVertexes(const std::vector<kmu::vec3> &vertexes) = 0;

	virtual void					SetUW(std::vector<kmu::vec2> && uw) = 0;
	virtual void					SetUW(const std::vector<kmu::vec2> &uw) = 0;

	virtual void					SetNormals(std::vector<kmu::vec3> && normals) = 0;
	virtual void					SetNormals(const std::vector<kmu::vec3> &normals) = 0;

	virtual std::vector<kmu::vec2>	GetUW() const = 0;
	virtual std::vector<uint32_t>	GetIndexes() const = 0;
	virtual std::vector<kmu::vec3>	GetNormals() const = 0;
	virtual std::vector<kmu::vec3>	GetVertexes() const = 0;

	virtual uint32_t				GetSizeInBytes() const = 0;

	virtual void					LoadFromFile(const std::string &filepath) = 0;

	virtual void					Submit() = 0;
	virtual void					Release() = 0;
	virtual bool					IsSubmited() const = 0;

	virtual const sAlmBoundingBox3D & GetBoundingBox() const = 0;

};

}


#endif // !_ALM_MESH_INTERFACE_HPP_