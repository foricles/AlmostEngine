#ifndef _ALM_ENTITY_MANAGER_HPP_
#define _ALM_ENTITY_MANAGER_HPP_

#include "alm_bintree.hpp"
#include "interface/alm_imgr.hpp"

namespace alme
{

class AlmEntityManager : public IAlmEntityManager, protected RBTree<IAlmEntity*>
{
public:
	AlmEntityManager(AlmostEngine *engine);
	~AlmEntityManager();

	IAlmEntity *CreateEntity(const std::string &name) override;
	void ReleaseEntity(IAlmEntity *entity) override;
	IAlmEntity *FindByName(const std::string &name) const override;

	IAlmTransform *GetRoot();
	void UpdateTransformationTree() override;

	void ReleaseAllEntities() override;
	void UpdateAllEntities() override;

protected:
	bool Compare(const Node *left, const Node *right) override;
	void UpdateAllEntities(Node *root);

private:
	IAlmTransform * m_rootTransform;

};

}


#endif // _ALM_ENTITY_MANAGER_HPP_