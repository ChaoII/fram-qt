#include "attend.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_EXPORT_DLL(Attend)

namespace qx {
template <> void register_class(QxClass<Attend> &t) {
    // 设置表名
    t.setName("Attend");
    // 注册 User::id <=> 数据库中的主键
    t.id(&Attend::id, "id");
    // 打卡人uid
    t.data(&Attend::uid, "uid");
    // 打卡人姓名
    t.data(&Attend::name, "name", 1);
    // 打卡时间
    t.data(&Attend::attend_time, "attend_tme");
}
} // namespace qx
