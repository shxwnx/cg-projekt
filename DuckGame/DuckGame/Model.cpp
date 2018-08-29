//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "PhongShader.h"
#include "DuckShader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}
Model::Model(const char* ModelFile, bool FitSize, float scale) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, FitSize, scale);
	if (!ret)
		throw std::exception();
}
Model::~Model()
{
	for (int i = MaterialCount - 1; i >= 0; i--) {
		pMaterials[i].~Material();
	}

	for (int i = MeshCount - 1; i >= 0; i--) {
		pMeshes[i].~Mesh();
	}

	pMaterials = NULL;
	pMeshes = NULL;

	deleteNodes(&RootNode);
}

Model::Model(Model * model)
{
	this->shader(new PhongShader());
	this->pMeshes = model->pMeshes;
	this->MeshCount = model->MeshCount;
	this->pMaterials = model->pMaterials;
	this->MaterialCount = model->MaterialCount;
	this->BoundingBox = model->BoundingBox;

	this->Filepath = model->Filepath;
	this->Path = model->Path;
	this->RootNode = model->RootNode;
}

void Model::deleteNodes(Node* pNode)
{
	if (!pNode)
		return;
	for (unsigned int i = 0; i < pNode->ChildCount; ++i)
		deleteNodes(&(pNode->Children[i]));
	if (pNode->ChildCount > 0)
		delete[] pNode->Children;
	if (pNode->MeshCount > 0)
		delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize, float scale)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

	if (pScene == NULL || pScene->mNumMeshes <= 0)
		return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, FitSize, scale);
	loadMaterials(pScene);
	loadNodes(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize, float scale)
{
	calcBoundingBox(pScene, this->BoundingBox, scale);

	float max = 2.0;
	if (FitSize) {														// Ermitteln, des größten Wertes der Bounding Box 
		if (max < fabs(BoundingBox.Max.X)) {
			max = fabs(BoundingBox.Max.X);
		}
		if (max < fabs(BoundingBox.Max.Y)) {
			max = fabs(BoundingBox.Max.Y);
		}
		if (max < fabs(BoundingBox.Max.Z)) {
			max = fabs(BoundingBox.Max.Z);
		}

		if (max < fabs(BoundingBox.Min.X)) {
			max = fabs(BoundingBox.Min.X);
		}
		if (max < fabs(BoundingBox.Min.Y)) {
			max = fabs(BoundingBox.Min.Y);
		}
		if (max < fabs(BoundingBox.Min.Z)) {
			max = fabs(BoundingBox.Min.Z);
		}
	}

	float factor = 2.0 / max;

	std::cout << "scale factor: " << factor << std::endl;
	MeshCount = pScene->mNumMeshes;
	this->pMeshes = new Mesh[MeshCount];								//Initialisierung der Meshes Liste

	for (int i = 0; i < MeshCount; i++) {								//Durchgehen aller Meshes aus pScene->mMeshes
		Mesh &tmpMesh = pMeshes[i];
		tmpMesh.MaterialIdx = pScene->mMeshes[i]->mMaterialIndex;

		tmpMesh.VB.begin();
		if (pScene->mMeshes[i]->HasPositions()) {						//Prüfen, ob Vertizes vorhanden sind
			for (int k = 0; k < pScene->mMeshes[i]->mNumVertices; k++) {
				aiVector3D vert = pScene->mMeshes[i]->mVertices[k];

				if (pScene->mMeshes[i]->HasNormals()) {					//Prüfen, ob Normalen vorhanden sind
					aiVector3D norm = pScene->mMeshes[i]->mNormals[k];
					tmpMesh.VB.addNormal(norm.x, norm.y, norm.z);
					//Hinzufügen der Normalen
				}

				if (pScene->mMeshes[i]->HasTextureCoords(0)) {			//Prüfen, ob Textur Koordinaten vorhanden sind
					float s = pScene->mMeshes[i]->mTextureCoords[0][k].x;
					float t = -(pScene->mMeshes[i]->mTextureCoords[0][k].y);
					tmpMesh.VB.addTexcoord0(s, t);						//Hinzufügen der Textur Koordinaten
				}
				tmpMesh.VB.addVertex(vert.x*factor, vert.y*factor, vert.z*factor);
				//Hinzufügen der skalierten Vertizes
			}
		}
		tmpMesh.VB.end();

		tmpMesh.IB.begin();
		if (pScene->mMeshes[i]->HasFaces()) {							//Prüfen, ob Flächen vorhanden sind
			for (int k = 0; k < pScene->mMeshes[i]->mNumFaces; k++) {	//Durchgehen aller Flächen
				tmpMesh.IB.addIndex(pScene->mMeshes[i]->mFaces[k].mIndices[0]);
				tmpMesh.IB.addIndex(pScene->mMeshes[i]->mFaces[k].mIndices[1]);
				tmpMesh.IB.addIndex(pScene->mMeshes[i]->mFaces[k].mIndices[2]);
				//Hinzufügen in den Index Buffer
			}
		}
		tmpMesh.IB.end();
	}
}

void Model::loadMaterials(const aiScene* pScene)
{
	MaterialCount = pScene->mNumMaterials;
	pMaterials = new Material[MaterialCount];											//Initialisierung der Material Liste

	if (pScene->HasTextures()) {
		std::cout << "has textures..." << std::endl;
	}

	if (pScene->HasMaterials()) {														//Prüfen, ob Materialen vorhanden sind
		std::cout << "has materials... " << pScene->mNumMaterials << std::endl;

		for (int matIdx = 0; matIdx < pScene->mNumMaterials; matIdx++) {				//Durchgehen aller Materialen
			aiMaterial* material = pScene->mMaterials[matIdx];
			aiColor4D specularColor;
			aiColor4D diffuseColor;
			aiColor4D ambientColor;
			float shininess;

			aiString path;
			material->Get(AI_MATKEY_NAME, path);										//Holen des Namens der Textur

			if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path)) {	//Holen der Textur
				std::string tmp = this->Path + path.data;
				Texture* text = new Texture(tmp.c_str());
				pMaterials[matIdx].DiffTex = text;
			}

			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor)) {	//Holen der diffusen Komponente
				pMaterials[matIdx].DiffColor = *new Color(diffuseColor.r, diffuseColor.g, diffuseColor.b);
			}

			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor)) {	//Holen der specular Komponente 
				pMaterials[matIdx].SpecColor = *new Color(specularColor.r, specularColor.g, specularColor.b);
			}

			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor)) {	//Holen der ambient Komponente
				pMaterials[matIdx].AmbColor = *new Color(ambientColor.r, ambientColor.g, ambientColor.b);
			}

			if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess)) {			//Holen des Glanzes
				pMaterials[matIdx].SpecExp = shininess;
			}

		}
	}
}

void Model::calcBoundingBox(const aiScene* pScene, AABB& Box, float scale)
{
	Vector	min(0, 0, 0);
	Vector	max(0, 0, 0);

	for (int mI = 0; mI < pScene->mNumMeshes; mI++)
	{
		aiMesh*  mesh = pScene->mMeshes[mI];
		for (int vI = 0; vI < mesh->mNumVertices; vI++)
		{
			aiVector3D vertex = mesh->mVertices[vI];

			if (vertex.x > max.X)	max.X = vertex.x;
			if (vertex.y > max.Y)	max.Y = vertex.y;
			if (vertex.z > max.Z)	max.Z = vertex.z;

			if (vertex.x < min.X)	min.X = vertex.x;
			if (vertex.y < min.Y)	min.Y = vertex.y;
			if (vertex.z < min.Z)	min.Z = vertex.z;
		}
	}

	Box.Min = min.operator*(scale);
	Box.Max = max.operator*(scale);
}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convert(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0)
		return;

	pNode->ChildCount = paiNode->mNumChildren;
	pNode->Children = new Node[pNode->ChildCount];
	for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
	{
		copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
		pNode->Children[i].Parent = pNode;
	}
}

void Model::applyMaterial(unsigned int index)
{
	if (index >= MaterialCount)
		return;

	PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
	DuckShader* pDuck = dynamic_cast<DuckShader*>(shader());
	if (pPhong) {
		Material* pMat = &pMaterials[index];
		pPhong->ambientColor(pMat->AmbColor);
		pPhong->diffuseColor(pMat->DiffColor);
		pPhong->specularExp(pMat->SpecExp);
		pPhong->specularColor(pMat->SpecColor);
		pPhong->diffuseTexture(pMat->DiffTex);
	}
	else if (pDuck) {
		Material* pMat = &pMaterials[index];
		pDuck->setAmbientColor(pMat->AmbColor);
		pDuck->setDiffuseColor(pMat->DiffColor);
		pDuck->setSpecularExp(pMat->SpecExp);
		pDuck->setSpecularColor(pMat->SpecColor);
		pDuck->setDiffuseTexture(pMat->DiffTex);
	}
	else {
		std::cout << "Model::applyMaterial() Wrong Shader!" << std::endl;
	}

}

void Model::draw(const BaseCamera& Cam)
{
	if (!pShader) {
		std::cout << "BaseModel::draw() no shader found" << std::endl;
		return;
	}
	pShader->modelTransform(transform());									//Merke die Transformation des Modells

	std::list<Node*> DrawNodes;
	DrawNodes.push_back(&RootNode);											//Speichere den Root in der Node-Liste

	while (!DrawNodes.empty())												//Wiederhole, bis die Node-Liste leer ist
	{
		Node* pNode = DrawNodes.front();									//Hole dir den ersten Node aus der Liste
		Matrix GlobalTransform;

		if (pNode->Parent != NULL)											//falls es ein Child ist...
			pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans; //rechne die Transformation des Parents drauf
		else																//falls es Root ist...
			pNode->GlobalTrans = transform() * pNode->Trans;				//merke dir die Transformation

		pShader->modelTransform(pNode->GlobalTrans);						//Speichern für Shader

		for (unsigned int i = 0; i < pNode->MeshCount; ++i)					//Aktualisieren jedes Models des Node
		{																	//Weiterleitung an die GPU
			Mesh& mesh = pMeshes[pNode->Meshes[i]];
			mesh.VB.activate();
			mesh.IB.activate();
			applyMaterial(mesh.MaterialIdx);
			pShader->activate(Cam);
			glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
			mesh.IB.deactivate();
			mesh.VB.deactivate();
		}
		for (unsigned int i = 0; i < pNode->ChildCount; ++i)					//Hole dir den nächsten Baumabschnitt in die Node-Liste (Children des aktuellen Nodes)
			DrawNodes.push_back(&(pNode->Children[i]));

		DrawNodes.pop_front();												//Löschen des aktuell ausgeführten Nodes 
	}
}

Matrix Model::convert(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}



