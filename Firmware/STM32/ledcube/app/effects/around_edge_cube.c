// Around Edge
// Run in one direction along the 12 edges of the cube.
#include <around_edge_cube.h>
#include <display_led.h>
#include <effects_manager.h>
#include "min.h"

extern volatile CubeMode_t current_mode;
extern volatile uint16_t   current_speed;

#define CHECK_EXIT                 \
    if (current_mode != MODE_EDGE) \
        return;

void Effect_Around_Edge(int delay_ms, int iterations)
{
    CHECK_EXIT

    for (int iter = 0; iter < iterations; iter++)
    {
        // Run along the bottom (Z=0)
        for (int i = 0; i < 7; i++)
        {
            Clear_Cube();
            Set_Voxel(i, 0, 0, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        } // X increases
        for (int i = 0; i < 7; i++)
        {
            Clear_Cube();
            Set_Voxel(7, i, 0, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        } // Y increases
        for (int i = 7; i > 0; i--)
        {
            Clear_Cube();
            Set_Voxel(i, 7, 0, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        } // X reduce
        for (int i = 7; i > 0; i--)
        {
            Clear_Cube();
            Set_Voxel(0, i, 0, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        } // Y reduce
          // Climb simultaneously on all four corners.
        for (int z = 0; z <= 7; z++)
        {
            Clear_Cube();
            Set_Voxel(0, 0, z, 1);
            Set_Voxel(7, 0, z, 1);
            Set_Voxel(7, 7, z, 1);
            Set_Voxel(0, 7, z, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
        // Runs next to the top (Z=7)
        for (int i = 0; i < 7; i++)
        {
            Clear_Cube();
            Set_Voxel(i, 0, 7, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
        for (int i = 0; i < 7; i++)
        {
            Clear_Cube();
            Set_Voxel(7, i, 7, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
        for (int i = 7; i > 0; i--)
        {
            Clear_Cube();
            Set_Voxel(i, 7, 7, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
        for (int i = 7; i > 0; i--)
        {
            Clear_Cube();
            Set_Voxel(0, i, 7, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
        // Falling
        for (int z = 7; z >= 0; z--)
        {
            Clear_Cube();
            Set_Voxel(0, 0, z, 1);
            Set_Voxel(7, 0, z, 1);
            Set_Voxel(7, 7, z, 1);
            Set_Voxel(0, 7, z, 1);
            Delay_Effect(current_speed);
            CHECK_EXIT
        }
    }
}
