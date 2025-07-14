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
    enum class Type { Minikit, SprayColor_RED, SprayColor_GREEN, SprayColor_BLUE };

    Type type;
    string name;
    string iconPath;
    string collectSound;

    ItemData(Type type, string name, string iconPath, string collectSound)
        : type(type), name(name), iconPath(iconPath), collectSound(collectSound) {}

    static ItemData Minikit() {
        return ItemData{
            Type::Minikit,
            "Minikit",
            "../Recursos/img/minikit.png",
            "../Recursos/audio/lego-star-wars-minikit.mp3"
        };
    }

    static ItemData SprayColor_RED() {
        return ItemData{
            Type::SprayColor_RED,
            "RED",
            "../Recursos/img/lata-vermelha.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData SprayColor_GREEN() {
        return ItemData{
            Type::SprayColor_GREEN,
            "GREEN",
            "../Recursos/img/lata-verde.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData SprayColor_BLUE() {
        return ItemData{
            Type::SprayColor_BLUE,
            "BLUE",
            "../Recursos/img/lata-azul.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }
};

#endif /* ItemData_hpp */
