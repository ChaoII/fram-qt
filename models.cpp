#include "models.h"

QX_REGISTER_CPP_EXPORT_DLL(Staff)
QX_REGISTER_CPP_EXPORT_DLL(Attend)
namespace qx {
    template<> void register_class(QxClass<Staff> &t){
        t.setName("staff");
        t.id(&Staff::id,"id");
        t.data(&Staff::uid,"uid");
        t.data(&Staff::name,"name");
        t.data(&Staff::pic_url,"pic_url");
        t.data(&Staff::feature,"feature");
    }
    template<> void register_class(QxClass<Attend> &t){
        t.setName("staff");
        t.id(&Attend::id,"id");
        t.data(&Attend::uid,"uid");
        t.data(&Attend::name,"name");
        t.data(&Attend::pic_url,"pic_url");
    }
}
