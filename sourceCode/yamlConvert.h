/*==========================================================*/
//
//	yamlConvert.h (YAMLÇÃå^í«â¡èàóù)
// 
// 
// ------------------------------ 2026/02/16 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*==========================================================*/

#ifndef _YAML_CONVERT_H
#define _YAML_CONVERT_H

#include "yaml-cpp/yaml.h"
#include "vector3.h"
#include "transform.h"

namespace YAML
{
	/* XMFLOAT2 */
	template<>
	struct convert<XMFLOAT2>
	{
		static Node encode(const XMFLOAT2& xmFloat2)
		{
			Node node;
			node["x"] = xmFloat2.x;
			node["y"] = xmFloat2.y;
			return node;
		}

		static bool decode(const Node& node, XMFLOAT2& xmFloat2)
		{
			if (!node.IsMap() || !node["x"] || !node["y"]) {
				return false;
			}

			xmFloat2.x = node["x"].as<float>();
			xmFloat2.y = node["y"].as<float>();
			return true;
		}
	};

	/* Vector3 */
	template<>
	struct convert<Vector3>
	{
		static Node encode(const Vector3& vector3)
		{
			Node node;
			node["x"] = vector3.x;
			node["y"] = vector3.y;
			node["z"] = vector3.z;
			return node;
		}

		static bool decode(const Node& node, Vector3& vector3)
		{
			if (!node.IsMap() || !node["x"] || !node["y"] || !node["z"]) {
				return false;
			}

			vector3.x = node["x"].as<float>();
			vector3.y = node["y"].as<float>();
			vector3.z = node["z"].as<float>();
			return true;
		}
	};



	/* Transform */
	template<>
	struct convert<Transform>
	{
		static Node encode(const Transform& transform)
		{
			Node node;
			node["position"] = transform.position;
			node["rotation"] = transform.rotation;
			node["scale"] = transform.scale;
			return node;
		}

		static bool decode(const Node& node, Transform& transform)
		{
			if (!node.IsMap() || !node["position"] || !node["rotation"] || !node["scale"]) {
				return false;
			}

			transform.position = node["position"].as<Vector3>();
			transform.rotation = node["rotation"].as<Vector3>();
			transform.scale = node["scale"].as<Vector3>();
			return true;
		}
	};
}

#endif //_YAML_CONVERT_H