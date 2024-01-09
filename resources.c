#include "resources.h"
#include <stdio.h>

ResourceContainer load_resources(const char* base_path)
{
    printf("loading resources...\n");

    ResourceContainer resource_container = CLITERAL(ResourceContainer) {};

    #define LOAD_TEXTURE(variable, relative_path) \
        const char* variable##_path = TextFormat("%s\\%s", base_path, relative_path); \
        printf("loading texture from: %s...\n", variable##_path); \
        resource_container.variable = CLITERAL(Resource) { \
            .path= variable##_path, \
            .texture= LoadTexture(variable##_path) \
        };

    LOAD_TEXTURE(player, "player.png");
    LOAD_TEXTURE(platform, "platform_default.png");
    LOAD_TEXTURE(platform_jumper, "platform_jumper.png");
    LOAD_TEXTURE(platform_bouncy, "platform_bouncy.png");

    printf("resources loaded successfully!\n");
    return resource_container;
}