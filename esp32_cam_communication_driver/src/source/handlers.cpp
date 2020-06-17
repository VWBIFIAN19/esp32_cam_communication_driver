#include "../headers/handlers.h"
#include "../headers/extensions.h"

#include "../headers/handlers/handler001requestpicture.h"

void Handlers::Setup() {
    /*Handle extensions*/
    Extensions::Setup();
}

void Handlers::Update() {
    /*Handler Request Picture*/
    Packets::HandlerRequestPicture::StreamPicture();

    /*Handle extensions*/
    Extensions::Update();
}