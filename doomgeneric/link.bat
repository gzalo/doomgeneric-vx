@set OLDPATH=%PATH%
@set VRXSDK=C:\eVoAps\SDK\1.2.0\vvsdk386
@set RVCTDIR=C:\Program Files\ARM\RVCT\Programs\4.0\400\win_32-pentium
@set PATH=%VRXSDK%\bin;%RVCTDIR%;C:\vrxsrc\bin

vrxcc -p -g build/doomvx.o build/dummy.o build/am_map.o build/doomdef.o build/doomstat.o build/dstrings.o build/d_event.o build/d_items.o build/d_iwad.o build/d_loop.o build/d_main.o build/d_mode.o build/d_net.o build/f_finale.o build/f_wipe.o build/g_game.o build/hu_lib.o build/hu_stuff.o build/info.o build/i_cdmus.o build/i_endoom.o build/i_joystick.o build/i_scale.o build/i_sound.o build/i_system.o build/i_timer.o build/memio.o build/m_argv.o build/m_bbox.o build/m_cheat.o build/m_config.o build/m_controls.o build/m_fixed.o build/m_menu.o build/m_misc.o build/m_random.o build/p_ceilng.o build/p_doors.o build/p_enemy.o build/p_floor.o build/p_inter.o build/p_lights.o build/p_map.o build/p_maputl.o build/p_mobj.o build/p_plats.o build/p_pspr.o build/p_saveg.o build/p_setup.o build/p_sight.o build/p_spec.o build/p_switch.o build/p_telept.o build/p_tick.o build/p_user.o build/r_bsp.o build/r_data.o build/r_draw.o build/r_main.o build/r_plane.o build/r_segs.o build/r_sky.o build/r_things.o build/sha1.o build/sounds.o build/statdump.o build/st_lib.o build/st_stuff.o build/s_sound.o build/tables.o build/v_video.o build/wi_stuff.o build/w_checksum.o build/w_file.o build/w_main.o build/w_wad.o build/z_zone.o build/w_file_stdc.o build/i_input.o build/i_video.o build/doomgeneric.o -o doom.out
vrxhdr -s 65536 -h 8388608 doom.out
pause
