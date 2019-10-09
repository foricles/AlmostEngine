#include <alm_almost_engine.hpp>
#include <almCore/almFileSystem/alm_file.hpp>
#include <iostream>

class SanboxApplication : public AlmEngineEntryPoint
{
public:
	void InitializeSsettings() override
	{
		std::cout << sizeof(alme::io::AlmFile) << std::endl;
		std::wcout << alme::io::AlmFileSystem::getRootPath() << std::endl;
		system("pause");
	}

	void RegisterGameScenes() override
	{
		alme::io::AlmFile sss(L"pidor.txt");
		//std::wcout << sss.GetFullPath()  << std::endl << (sss.Exist() ? sss.Load() : "hui") << std::endl;
		std::cout << "start load" << std::endl;
		sss.LoadAsync();
		std::wcout << sss.GetContentAsync() << std::endl;
		system("pause");
	}
};

AlmEngineEntryPoint *CreateEntryPoint()
{
	return new SanboxApplication();
}