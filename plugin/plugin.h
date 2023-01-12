#ifndef PLUGIN_H
#define PLUGIN_H

class Plugin {
public:
    Plugin() = default;
    static void register_orm();
    static void register_facehelper();
    static void register_vectorhelper();
};

#endif // PLUGIN_H
