#include "resources.h"
#include <stdio.h>

ResourceContainer load_resources(const char* base_path)
{
    printf("loading resources...\n");

    ResourceContainer resource_container = {
        player: {
            path: TextFormat("%s\\player.png", base_path)
        },
        platform: {
            path: TextFormat("%s\\platform.png", base_path)
        },
    };
    resource_container.player.texture = LoadTexture(resource_container.player.path);
    resource_container.platform.texture = LoadTexture(resource_container.platform.path);

    printf("resources loaded successfully!\n");
    return resource_container;
}