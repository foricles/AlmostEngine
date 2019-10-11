#ifndef _ALM_ENTITY_MANAGER_HPP_
#define _ALM_ENTITY_MANAGER_HPP_

#include "alm_imgr.hpp"
#include "alm_bintree.hpp"

namespace alme
{

class AlmEntityManager : public IAlmEntityManager, protected RBTree<IAlmEntity*>
{
public:
	AlmEntityManager();
	virtual ~AlmEntityManager();

	IAlmEntity *CreateEntity(const std::string &name) override;
	void ReleaseEntity(IAlmEntity *entity) override;
	IAlmEntity *FindByName(const std::string &name) const override;

protected:
	bool Compare(const Node *left, const Node *right) override;

};

}


#endif // _ALM_ENTITY_MANAGER_HPP_