#include "models.h"

QX_REGISTER_CPP_EXPORT_DLL(Staff)
QX_REGISTER_CPP_EXPORT_DLL(Attend)
namespace qx {
    template<> void register_class(QxClass<Staff> &t){
        t.setName("staff");
        t.id(&Staff::id,"id");
        t.data(&Staff::index_id,"index_id");
        t.data(&Staff::uid,"uid");
        t.data(&Staff::name,"name");
        t.data(&Staff::pic_url,"pic_url");
        t.data(&Staff::feature,"feature");
        t.data(&Staff::register_time,"register_time");
    }
    template<> void register_class(QxClass<Attend> &t){
        t.setName("attend");
        t.id(&Attend::id, "id");
        t.data(&Attend::uid, "uid");
        t.data(&Attend::name, "name");
        t.data(&Attend::pic_url, "pic_url");
        t.data(&Attend::attend_time, "attend_time");
    }
}
