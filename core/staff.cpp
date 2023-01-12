#include "staff.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_EXPORT_DLL(Staff)

namespace qx {
template <> void register_class(QxClass<Staff> &t) {
    // 设置表名
    t.setName("Staff");
    // 注册 User::id <=> 数据库中的主键
    auto id_=t.id(&Staff::id, "id");
    // 设置自定义id必备
    id_->setAutoIncrement(false);
    // 人员uid
    t.data(&Staff::uid, "uid");
    // 注册 User::name 属性，使用的 key 是 name，version 是 1。
    t.data(&Staff::name, "name", 1);
    // 注册 User::age 属性，使用的 key 是 picture
    t.data(&Staff::picture, "picture");
    // 注册 User::hobbies 属性，使用的 key 是 feature
    t.data(&Staff::feature, "feature");
}
} // namespace qx
