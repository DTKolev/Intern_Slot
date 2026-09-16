#include "../headers/Singleton_Common.hpp"

using namespace single;

Error::Error() : message{"Unspecified error"} {}

Error::Error(std::string msg) : message{msg} {}

auto Error::GetMessage() const -> const std::string& {return message;}



FailedSurfaceCreate::FailedSurfaceCreate() : Error("Error creating a surface") {}

FailedTextureCreate::FailedTextureCreate() : Error("Error creating a texture") {}
