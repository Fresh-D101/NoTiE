#pragma once
#include <unordered_map>
#include <filesystem>
#include <system_error>
#include <iostream>

#include "../Core/Utils.h"

template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;
	
	virtual void StartUp(const std::string basePath)
	{
		LoadPaths(basePath);
	}

	virtual void ShutDown()
	{
		Resources.clear();
		Paths.clear();
	}

	T* GetResource(const std::string& name)
	{
		auto resource = Find(name);
		if(resource)
		{
			++resource->second;
			return resource->first.get();
		}

		return nullptr;
	}

	bool IsLoaded(const std::string& name)
	{
		auto iterator = Resources.find(name);
		return (iterator != Resources.end());
	}

	T* Load(const std::string& name)
	{		
		return static_cast<Derived*>(this)->Load(name);
	}
	
	std::string GetPath(const std::string& name)
	{
		auto path = Paths.find(name);
		return (path != Paths.end() ? path->second : "");
	}
	 
	bool ReleaseResource(const std::string& name)
	{
		auto resource = Find(name);
		if(!resource)
		{
			return false;
		}
		--resource->second;
		
		if(resource->second <= 0)
		{
			Unload(name);
		}

		return true;
	}

private:
	bool Unload(const std::string& name)
	{
		auto iterator = Resources.find(name);
		if (iterator == Resources.end())
		{
			return false;
		}

		Resources.erase(iterator);
		return true;
	}
	
	std::pair<std::unique_ptr<T>, int>* Find(const std::string& name)
	{
		auto iterator = Resources.find(name);
		return (iterator != Resources.end() ? &iterator->second : nullptr);
	}
	
	void LoadPaths(const std::string basePath)
	{
		std::filesystem::recursive_directory_iterator iterator(Utils::GetWorkingDirectory() + basePath);
		std::filesystem::recursive_directory_iterator end;

		while(iterator != end)
		{
			std::string pathName = iterator->path().string();
			std::string fileName = iterator->path().stem().string();

			Paths.emplace(fileName, pathName);

			std::error_code ec;
			iterator.increment(ec);
			if(ec)
			{
				std::cerr << "Error While Accessing : " << iterator->path().string() << " :: " << ec.message() << std::endl;
			}
		}
	}

protected:
	std::unordered_map<std::string, std::pair<std::unique_ptr<T>, int>> Resources;
	std::unordered_map<std::string, std::string> Paths;
};
