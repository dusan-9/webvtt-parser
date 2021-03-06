

namespace webvtt {

template<typename Object>
std::unique_ptr<Object> ObjectParser<Object>::collectCurrentObject() {
  std::unique_ptr<Object> temp = std::move(currentObject);
  currentObject = nullptr;
  return temp;
};

template<typename Object>
bool ObjectParser<Object>::setNewObjectForParsing(std::unique_ptr<Object> newObject) {
  if (currentObject != nullptr)
    return false;
  currentObject = std::move(newObject);
  return true;
};

template<typename Object>
const Object *
ObjectParser<Object>::getCurrentParsedObject() {
  return currentObject.get();
}

template<typename Object>
ObjectParser<Object>::~ObjectParser<Object>() = default;

}