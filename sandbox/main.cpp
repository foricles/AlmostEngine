#include <alm_almost_engine.hpp>
#include <almCore/almFileSystem/alm_file.hpp>
#include <almCore/almEntitySystem/almEntity/alm_entity.hpp>
#include <iostream>

class SanboxApplication : public AlmEngineEntryPoint
{
public:
	void InitializeSsettings() override
	{
		std::cout << sizeof(alme::io::AlmFile) << std::endl;
		std::wcout << alme::io::AlmFileSystem::getRootPath() << std::endl;
		alme::io::AlmFile sss(L"Sandbox.pdb");
		//std::wcout << sss.GetFullPath()  << std::endl << (sss.Exist() ? sss.Load() : "hui") << std::endl;
		std::cout << "start load" << std::endl;
		sss.LoadAsync();
		std::wcout << sss.GetContentAsync() << std::endl;
		system("pause");
	}

	void RegisterGameScenes() override
	{
		alme::AlmEntity a1;
		alme::AlmEntity a2;
		alme::AlmEntity a3;
		a2.GetTransform().AddChild(&a1.GetTransform());
		a1.GetTransform().AddChild(&a3.GetTransform());
		a3.GetTransform().AddChild(&a1.GetTransform());
		int a = 6;
		system("pause");
	}
};

AlmEngineEntryPoint *CreateEntryPoint()
{
	return new SanboxApplication();
}