#include "stdafx.h"
#include "Scene.h"
#include "Manager.h"

using namespace ManagedFbx;

Scene::Scene(string ^name)
{
	m_nativeScene = FbxScene::Create(Manager::GetInstance(), StringHelper::ToNative(name));
	m_rootNode = gcnew SceneNode(m_nativeScene->GetRootNode());
}

Scene ^Scene::Import(string ^filename)
{
	auto importer = Manager::GetImporter();

	if(!importer->Initialize(StringHelper::ToNative(filename)))
		throw gcnew FbxException("Failed to initialise the FBX importer: {0}", gcnew string(importer->GetLastErrorString()));

	auto scene = gcnew Scene("");

	if(!importer->Import(scene->m_nativeScene))
		throw gcnew FbxException("Failed to import the scene: {0}", gcnew string(importer->GetLastErrorString()));

	// Needs refreshing
	scene->m_rootNode = gcnew SceneNode(scene->m_nativeScene->GetRootNode());

	return scene;
}

void Scene::Save(string ^filename)
{
	auto exporter = Manager::GetExporter();

	if(!exporter->Initialize(StringHelper::ToNative(filename)))
		throw gcnew FbxException("Failed to initialise the FBX exporter: {0}", gcnew string(exporter->GetLastErrorString()));

	if(!exporter->Export(m_nativeScene))
		throw gcnew FbxException("Failed to export the scene: {0}", gcnew string(exporter->GetLastErrorString()));
}

void Scene::Name::set(string ^value)
{
	m_nativeScene->SetName(StringHelper::ToNative(value));
}

string ^Scene::Name::get()
{
	return gcnew string(m_nativeScene->GetName());
}

SceneNode ^Scene::RootNode::get()
{
	return m_rootNode;
}