#include <gba.h> 

#include "sprites.h" 
#include "background.h"
#include "random.h"
#include "enemies.h"
#include "bullets.h"
#include "explosions.h"
#include "entities.h"
#include "audio.h"

#define IS_KEY_PRESSED(k) (!(REG_KEYINPUT & (k)))

typedef struct {
    Entity* entity;
    int ticksUntilShoot;
    int hp;
    bool dead;
    int ticksUntilSpawn;
    int ticksInvulnerability;
} Player;

Player sm_player;

const int DELAY_SHOOT_TICKS = 10;

void game_process_input()
{
    if (sm_player.dead)
        return;

    // handle ship movement
    int deltaX = 0, deltaY = 0;
    if (IS_KEY_PRESSED(KEY_UP))
        deltaY--;
    if (IS_KEY_PRESSED(KEY_DOWN))
        deltaY++;
    if (IS_KEY_PRESSED(KEY_LEFT))
        deltaX--;
    if (IS_KEY_PRESSED(KEY_RIGHT))
        deltaX++;

    sm_player.entity->x += deltaX;
    sm_player.entity->y += deltaY;

    if (IS_KEY_PRESSED(KEY_A))
    {
        if (sm_player.ticksUntilShoot <= 0)
        {
            audio_play_laser();
            bullet_create(sm_player.entity->x, sm_player.entity->y);
            sm_player.ticksUntilShoot = DELAY_SHOOT_TICKS;
        }
    }
}

void game_spawn_player()
{
    sm_player.entity->x = 10;
    sm_player.entity->y = 100;
    sm_player.hp = 1;
    sm_player.dead = false;
    sm_player.ticksInvulnerability = 15;
}

void game_init()
{
    sm_player.entity = entity_create(ENTITY_PLAYER, &sm_player);
    entity_set_sprite(sm_player.entity, 0);
    collision_init(&(sm_player.entity->collider), 0, 0, 16, 16);
    game_spawn_player();
    background_set_scroll_speed(1);
}

void game_kill_player()
{
    explosion_create(sm_player.entity->x, sm_player.entity->y);
    sm_player.dead = true;
    sm_player.ticksUntilSpawn = 30;
    sm_player.entity->x = -16;
}

void game_update()
{
    if (sm_player.ticksUntilShoot > 0)
        --sm_player.ticksUntilShoot;

    if (sm_player.ticksInvulnerability <= 0)
    {
        const int numEnemies = entities_get_count(ENTITY_ENEMY);
        for (int i = 0; i < numEnemies; ++i)
        {
            Entity* entity_enemy = entities_get(ENTITY_ENEMY, i);
            if (entity_enemy && collision_test(entity_enemy, sm_player.entity))
            {
                sm_player.hp -= 1;
            }
        }
    }
    else
    {
        sm_player.ticksInvulnerability--;
    }

    if (!sm_player.dead && sm_player.hp <= 0)
    {
        background_set_scroll_speed(0);
        game_kill_player();
    }

    if (sm_player.dead)
    {
        sm_player.ticksUntilSpawn--;
        if (sm_player.ticksUntilSpawn <= 0)
        {
            background_set_scroll_speed(1);
            game_spawn_player();
        }
    }
}

int game_main()
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    random_init(31173);

    REG_DISPCNT = MODE_0 | OBJ_1D_MAP;
    REG_DISPCNT |= OBJ_ENABLE;
    REG_DISPCNT |= BG0_ENABLE | BG1_ENABLE;

    sprites_init_OAM();
    sprites_load_sprite_sheet();
    background_init();

    audio_init();

    game_init();

    enemies_init_all();

    audio_play_music();

    while (1)
    {
        audio_update();

        game_process_input();

        bullets_update_all();
        enemies_update_all();
        explosions_update_all();
        entities_update_all();

        game_update();

        VBlankIntrWait();

        sprites_update_OAM();
        background_update();
    }
}
