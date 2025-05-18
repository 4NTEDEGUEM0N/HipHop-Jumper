//
//  ItemData.hpp
//  hhj
//
//  Created by Pedro Venzi on 18/05/25.
//

#ifndef ItemData_hpp
#define ItemData_hpp

#include <stdio.h>
#include <string>

using namespace std;

struct ItemData {
    enum class Type { Minikit, SprayCap, SprayBody, SprayColor };

    Type type;
    string name;
    string iconPath;
    int maxStack;

    ItemData(Type type, string name, string iconPath, int maxStack)
        : type(type), name(name), iconPath(iconPath), maxStack(maxStack) {}

    static ItemData Minikit() {
        return ItemData{
            Type::Minikit,
            "Minikit",
            "../Recursos/img/minikit.png",
            1
        };
    }
};

#endif /* ItemData_hpp */
