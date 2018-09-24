#pragma once

#include <memory>

/** Usage example
int main(){

//shared pointer
sp<EG_Class> ptr2 = new_sp<EG_Class>();
ptr2->Foo();

//weak pointer
wp<EG_Class> wp1 = ptr2;
if (sp<EG_Class> aquired = wp1.lock())
{
aquired->Foo();
}

//unique pointer
up<EG_Class> up1 = new_up<EG_Class>();
up1->Foo();
}
*/

//option 1 for shared pointers
template<typename T>
using sp = std::shared_ptr<T>;

//template<typename T>
//const auto new_sp = std::make_shared<T>; //this has issues with arguments, perhaps need fptr with all signatures?

template<typename T, typename... Args>
const sp<T> new_sp(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

//option 2 for shared pointers
template<typename T>
using sptr = std::shared_ptr<T>;

//template<typename T>
//const auto new_sptr = std::make_shared<T>;

//template<typename T, typename... Args>
//const sp<T> new_sptr(Args&&... args)
//{
//	return std::make_shared<T>(std::forward<Args>(args)...);
//}




//weak pointers
template<typename T>
using wp = std::weak_ptr<T>;

//unique pointers
template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T>
using up = std::unique_ptr<T>;

template<typename T, typename... Args>
const sp<T> new_up(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
const auto shared_this = std::shared_from_this; //untested, may need 