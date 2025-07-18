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
    enum class Type { Minikit, Minikit2, Minikit3, SprayColor_RED, SprayColor_GREEN, SprayColor_BLUE, MarkerColor_RED, MarkerColor_GREEN, MarkerColor_BLUE, BucketColor_RED, BucketColor_GREEN, BucketColor_BLUE };

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
            "../Recursos/img/COLETAVEISSCALED/SPRITE LATA LARANJA.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData SprayColor_GREEN() {
        return ItemData{
            Type::SprayColor_GREEN,
            "GREEN",
            "../Recursos/img/COLETAVEISSCALED/SPRITE LATA VERDE.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData SprayColor_BLUE() {
        return ItemData{
            Type::SprayColor_BLUE,
            "BLUE",
            "../Recursos/img/COLETAVEISSCALED/SPRITE LATA ROXA.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }
    
    static ItemData MarkerColor_RED() {
        return ItemData{
            Type::MarkerColor_RED,
            "RED",
            "../Recursos/img/COLETAVEISSCALED/SPRITE MARCADOR LARANJA.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData MarkerColor_GREEN() {
        return ItemData{
            Type::MarkerColor_GREEN,
            "GREEN",
            "../Recursos/img/COLETAVEISSCALED/SPRITE MARCADOR VERDE.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData MarkerColor_BLUE() {
        return ItemData{
            Type::MarkerColor_BLUE,
            "BLUE",
            "../Recursos/img/COLETAVEISSCALED/SPRITE MARCADOR ROXO.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }
    
    static ItemData BucketColor_RED() {
        return ItemData{
            Type::BucketColor_RED,
            "RED",
            "../Recursos/img/COLETAVEISSCALED/SPRITE BALDE LARANJA.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData BucketColor_GREEN() {
        return ItemData{
            Type::BucketColor_GREEN,
            "GREEN",
            "../Recursos/img/COLETAVEISSCALED/SPRITE BALDE VERDE.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }

    static ItemData BucketColor_BLUE() {
        return ItemData{
            Type::SprayColor_BLUE,
            "BLUE",
            "../Recursos/img/COLETAVEISSCALED/SPRITE BALDE ROXO.png",
            "../Recursos/audio/INTERACOES/COLLECT.wav"
        };
    }
};

#endif /* ItemData_hpp */
