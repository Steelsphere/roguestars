#pragma once
#include <libtcod.hpp>
#include <vector>
#include <typeinfo>

namespace Serializer {

template <typename T>
inline void serialize_vector(TCODZip* zip, std::vector<T> vec) {
	int size = vec.size();
	zip->putInt(size);
		for (T& val : vec) {
			val.serialize(zip);
		}
}

template <typename T>
inline std::vector<T> deserialize_vector(TCODZip* zip) {
	int size = zip->getInt();
	std::vector<T> new_vec;
	new_vec.resize(size);
	
	for (int i = 0; i < new_vec.size(); i++) {
			T new_type;
			new_type.deserialize(zip);
			new_vec[i] = new_type;
		}
	
	return new_vec;
}

template <typename T>
inline std::vector<T*> deserialize_vector(TCODZip* zip, bool pointer) {
	int size = zip->getInt();
	std::vector<T*> new_vec;
	new_vec.resize(size);

	for (int i = 0; i < new_vec.size(); i++) {
		T* new_type = new T;
		new_type->deserialize(zip);
		new_vec[i] = new_type;
	}

	return new_vec;
}

template <typename T>
inline std::vector<T> pointer_vector_to_reference(std::vector<T*> vec) {
	std::vector<T> new_vec;
	for (T* val : vec) {
		new_vec.push_back(*val);
	}
	return new_vec;
}

}