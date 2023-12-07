#include "Mesh.h"

#include "Triangle.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

bool Mesh::Hit( const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit ) {

	for ( int i = 0; i < m_vertices.size(); i+= 3 ) {

		Triangle triangle( m_vertices[i], m_vertices[i + 1], m_vertices[i + 2], m_material);
		if ( triangle.Hit( ray, minDistance, maxDistance, raycastHit ) ) return true;

	}

	return false;

}

bool Mesh::Load( const std::string& filename, const glm::vec3& translate, const glm::vec3& rotation, const glm::vec3& scale ) {

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );
	if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {

		std::cerr << "Could not load assimp file " << filename << " error string: " << importer.GetErrorString();
		return false;

	}

	glm::mat4 mt = glm::translate( translate );
	glm::mat4 mr = glm::eulerAngleYXZ( glm::radians( rotation.y ), glm::radians( rotation.x ), glm::radians( rotation.z ) );
	glm::mat4 ms = glm::scale( scale );

	glm::mat4 mx = mt * mr * ms;

	ProcessNode( scene->mRootNode, scene, mx );

	return true;

}

void Mesh::ProcessNode( aiNode* node, const aiScene* scene, const glm::mat4& transform ) {

	for ( unsigned int i = 0; i < node->mNumMeshes; i++ ) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh( mesh, scene, transform );

	}

	for ( unsigned int i = 0; i < node->mNumChildren; i++ ) {

		ProcessNode( node->mChildren[i], scene, transform );

	}

}

void Mesh::ProcessMesh( aiMesh* mesh, const aiScene* scene, const glm::mat4& transform ) {

	std::vector<glm::vec3> positions;

	for ( unsigned int i = 0; i < mesh->mNumVertices; i++ ) {

		glm::vec3 position = transform * glm::vec4 { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1 };
		positions.push_back( position );

	}


	for ( size_t i = 0; i < mesh->mNumFaces; i++ ) {

		aiFace face = mesh->mFaces[i];
		for ( size_t j = 0; j < face.mNumIndices; j++ ) {

			unsigned int index = face.mIndices[j];
			m_vertices.push_back( positions[index] );

		}

	}

}