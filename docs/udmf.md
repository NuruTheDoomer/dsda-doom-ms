## UDMF (Universal Doom Map Format)

This page tracks support for the universal doom map format, as seen in ZDoom. It is currently possible to parse and load maps that don't violate existing engine constraints (for instance, using flats as textures). The different features are being evaluated and implemented one by one. The status for any feature, including whether or not it will be supported in the future, is subject to change. See the [Doom in Hexen docs](./doom_in_hexen.md) for more information about which specials and thing types are supported.

### Legend

| Symbol             | Meaning                        |
| ------------------ | ------------------------------ |
| :heavy_check_mark: | Feature is fully supported     |
| :warning:          | Feature is partially supported |
| :grey_question:    | Feature is under investigation |
| :telescope:        | Feature is planned             |
| :comet:            | Feature is planned later       |
| :x:                | Feature is not planned         |

### Namespaces

| Name       | Status    |
| ---------- | --------- |
| doom       | :x:       |
| heretic    | :x:       |
| hexen      | :x:       |
| strife     | :x:       |
| zdoom 1.33 | :warning: |

### Nodes

| Name          | Status             |
| ------------- | ------------------ |
| ZDBSP X/Z GLN | :heavy_check_mark: |
| ZDBSP X/Z GL2 | :grey_question:    |
| ZDBSP X/Z GL3 | :grey_question:    |

### Linedefs

| Field              | Status             |
| ------------------ | ------------------ |
| id                 | :heavy_check_mark: |
| v1                 | :heavy_check_mark: |
| v2                 | :heavy_check_mark: |
| blocking           | :heavy_check_mark: |
| blockmonsters      | :heavy_check_mark: |
| twosided           | :heavy_check_mark: |
| dontpegtop         | :heavy_check_mark: |
| dontpegbottom      | :heavy_check_mark: |
| secret             | :heavy_check_mark: |
| blocksound         | :heavy_check_mark: |
| dontdraw           | :heavy_check_mark: |
| mapped             | :heavy_check_mark: |
| passuse            | :heavy_check_mark: |
| translucent        | :heavy_check_mark: |
| jumpover           | :heavy_check_mark: |
| blockfloaters      | :heavy_check_mark: |
| playercross        | :heavy_check_mark: |
| playeruse          | :heavy_check_mark: |
| monstercross       | :heavy_check_mark: |
| monsteruse         | :heavy_check_mark: |
| impact             | :heavy_check_mark: |
| playerpush         | :heavy_check_mark: |
| monsterpush        | :heavy_check_mark: |
| missilecross       | :heavy_check_mark: |
| repeatspecial      | :heavy_check_mark: |
| special            | :heavy_check_mark: |
| arg0               | :warning:          |
| arg1               | :warning:          |
| arg2               | :warning:          |
| arg3               | :warning:          |
| arg4               | :warning:          |
| sidefront          | :heavy_check_mark: |
| sideback           | :heavy_check_mark: |
| comment            | :x:                |
| alpha              | :heavy_check_mark: |
| renderstyle        | :x:                |
| playeruseback      | :heavy_check_mark: |
| anycross           | :heavy_check_mark: |
| monsteractivate    | :heavy_check_mark: |
| blockplayers       | :heavy_check_mark: |
| blockeverything    | :heavy_check_mark: |
| firstsideonly      | :heavy_check_mark: |
| zoneboundary       | :x:                |
| clipmidtex         | :heavy_check_mark: |
| wrapmidtex         | :telescope:        |
| midtex3d           | :heavy_check_mark: |
| midtex3dimpassible | :heavy_check_mark: |
| checkswitchrange   | :heavy_check_mark: |
| blockprojectiles   | :heavy_check_mark: |
| blockuse           | :heavy_check_mark: |
| blocksight         | :heavy_check_mark: |
| blockhitscan       | :heavy_check_mark: |
| locknumber         | :heavy_check_mark: |
| arg0str            | :x:                |
| moreids            | :heavy_check_mark: |
| transparent        | :heavy_check_mark: |
| automapstyle       | :heavy_check_mark: |
| revealed           | :heavy_check_mark: |
| noskywalls         | :x:                |
| drawfullheight     | :x:                |
| health             | :heavy_check_mark: |
| healthgroup        | :heavy_check_mark: |
| damagespecial      | :heavy_check_mark: |
| deathspecial       | :heavy_check_mark: |
| blocklandmonsters  | :heavy_check_mark: |

#### Notes
- `arg*` currently has hexen's limit of 0-255.

### Sidedefs

| Field                 | Status             |
| --------------------- | ------------------ |
| offsetx               | :heavy_check_mark: |
| offsety               | :heavy_check_mark: |
| texturetop            | :heavy_check_mark: |
| texturebottom         | :heavy_check_mark: |
| texturemiddle         | :heavy_check_mark: |
| sector                | :heavy_check_mark: |
| comment               | :x:                |
| scalex_top            | :comet:            |
| scaley_top            | :comet:            |
| scalex_mid            | :comet:            |
| scaley_mid            | :comet:            |
| scalex_bottom         | :comet:            |
| scaley_bottom         | :comet:            |
| offsetx_top           | :heavy_check_mark: |
| offsety_top           | :heavy_check_mark: |
| offsetx_mid           | :heavy_check_mark: |
| offsety_mid           | :heavy_check_mark: |
| offsetx_bottom        | :heavy_check_mark: |
| offsety_bottom        | :heavy_check_mark: |
| light                 | :heavy_check_mark: |
| light_top             | :heavy_check_mark: |
| light_mid             | :heavy_check_mark: |
| light_bottom          | :heavy_check_mark: |
| lightabsolute         | :heavy_check_mark: |
| lightabsolute_top     | :heavy_check_mark: |
| lightabsolute_mid     | :heavy_check_mark: |
| lightabsolute_bottom  | :heavy_check_mark: |
| lightfog              | :x:                |
| nofakecontrast        | :heavy_check_mark: |
| smoothlighting        | :heavy_check_mark: |
| clipmidtex            | :heavy_check_mark: |
| wrapmidtex            | :telescope:        |
| nodecals              | :x:                |
| nogradient_top        | :x:                |
| flipgradient_top      | :x:                |
| clampgradient_top     | :x:                |
| useowncolors_top      | :x:                |
| uppercolor_top        | :x:                |
| lowercolor_top        | :x:                |
| nogradient_mid        | :x:                |
| flipgradient_mid      | :x:                |
| clampgradient_mid     | :x:                |
| useowncolors_mid      | :x:                |
| uppercolor_mid        | :x:                |
| lowercolor_mid        | :x:                |
| nogradient_bottom     | :x:                |
| flipgradient_bottom   | :x:                |
| clampgradient_bottom  | :x:                |
| useowncolors_bottom   | :x:                |
| uppercolor_bottom     | :x:                |
| lowercolor_bottom     | :x:                |
| useowncoloradd_top    | :x:                |
| useowncoloradd_mid    | :x:                |
| useowncoloradd_bottom | :x:                |
| coloradd_top          | :x:                |
| coloradd_mid          | :x:                |
| coloradd_bottom       | :x:                |
| colorization_top      | :x:                |
| colorization_mid      | :x:                |
| colorization_bottom   | :x:                |

### Vertices

| Field    | Status             |
| -------- | ------------------ |
| x        | :heavy_check_mark: |
| y        | :heavy_check_mark: |
| zfloor   | :x:                |
| zceiling | :x:                |

### Sectors

| Field                    | Status             |
| ------------------------ | ------------------ |
| heightfloor              | :heavy_check_mark: |
| heightceiling            | :heavy_check_mark: |
| texturefloor             | :heavy_check_mark: |
| textureceiling           | :heavy_check_mark: |
| lightlevel               | :heavy_check_mark: |
| special                  | :heavy_check_mark: |
| id                       | :heavy_check_mark: |
| comment                  | :x:                |
| xpanningfloor            | :heavy_check_mark: |
| ypanningfloor            | :heavy_check_mark: |
| xpanningceiling          | :heavy_check_mark: |
| ypanningceiling          | :heavy_check_mark: |
| xscalefloor              | :comet:            |
| yscalefloor              | :comet:            |
| xscaleceiling            | :comet:            |
| yscaleceiling            | :comet:            |
| rotationfloor            | :heavy_check_mark: |
| rotationceiling          | :heavy_check_mark: |
| ceilingplane_a           | :x:                |
| ceilingplane_b           | :x:                |
| ceilingplane_c           | :x:                |
| ceilingplane_d           | :x:                |
| floorplane_a             | :x:                |
| floorplane_b             | :x:                |
| floorplane_c             | :x:                |
| floorplane_d             | :x:                |
| lightfloor               | :heavy_check_mark: |
| lightceiling             | :heavy_check_mark: |
| lightfloorabsolute       | :heavy_check_mark: |
| lightceilingabsolute     | :heavy_check_mark: |
| alphafloor               | :x:                |
| alphaceiling             | :x:                |
| renderstylefloor         | :x:                |
| renderstyleceiling       | :x:                |
| gravity                  | :heavy_check_mark: |
| lightcolor               | :x:                |
| fadecolor                | :x:                |
| desaturation             | :x:                |
| silent                   | :heavy_check_mark: |
| nofallingdamage          | :x:                |
| noattack                 | :heavy_check_mark: |
| dropactors               | :x:                |
| norespawn                | :x:                |
| soundsequence            | :x:                |
| hidden                   | :heavy_check_mark: |
| waterzone                | :x:                |
| moreids                  | :heavy_check_mark: |
| damageamount             | :heavy_check_mark: |
| damagetype               | :x:                |
| damageinterval           | :heavy_check_mark: |
| leakiness                | :heavy_check_mark: |
| damageterraineffect      | :x:                |
| damagehazard             | :heavy_check_mark: |
| floorterrain             | :x:                |
| ceilingterrain           | :x:                |
| portal_ceil_blocksound   | :x:                |
| portal_ceil_disabled     | :x:                |
| portal_ceil_nopass       | :x:                |
| portal_ceil_norender     | :x:                |
| portal_ceil_overlaytype  | :x:                |
| portal_floor_blocksound  | :x:                |
| portal_floor_disabled    | :x:                |
| portal_floor_nopass      | :x:                |
| portal_floor_norender    | :x:                |
| portal_floor_overlaytype | :x:                |
| floor_reflect            | :x:                |
| ceiling_reflect          | :x:                |
| fogdensity               | :x:                |
| floorglowcolor           | :x:                |
| floorglowheight          | :x:                |
| ceilingglowcolor         | :x:                |
| ceilingglowheight        | :x:                |
| color_floor              | :x:                |
| color_ceiling            | :x:                |
| color_walltop            | :x:                |
| color_wallbottom         | :x:                |
| color_sprites            | :x:                |
| coloradd_floor           | :x:                |
| coloradd_ceiling         | :x:                |
| coloradd_sprites         | :x:                |
| coloradd_walls           | :x:                |
| colorization_floor       | :x:                |
| colorization_ceiling     | :x:                |
| noskywalls               | :x:                |
| healthfloor              | :x:                |
| healthfloorgroup         | :x:                |
| healthceiling            | :x:                |
| healthceilinggroup       | :x:                |

### Things

| Field         | Status             |
| ------------- | ------------------ |
| id            | :heavy_check_mark: |
| x             | :heavy_check_mark: |
| y             | :heavy_check_mark: |
| height        | :heavy_check_mark: |
| angle         | :heavy_check_mark: |
| type          | :heavy_check_mark: |
| skill1        | :warning:          |
| skill2        | :warning:          |
| skill3        | :heavy_check_mark: |
| skill4        | :warning:          |
| skill5        | :warning:          |
| ambush        | :heavy_check_mark: |
| single        | :heavy_check_mark: |
| dm            | :heavy_check_mark: |
| coop          | :heavy_check_mark: |
| friend        | :heavy_check_mark: |
| dormant       | :heavy_check_mark: |
| class1        | :x:                |
| class2        | :x:                |
| class3        | :x:                |
| standing      | :x:                |
| strifeally    | :x:                |
| translucent   | :heavy_check_mark: |
| invisible     | :heavy_check_mark: |
| special       | :heavy_check_mark: |
| arg0          | :warning:          |
| arg1          | :warning:          |
| arg2          | :warning:          |
| arg3          | :warning:          |
| arg4          | :warning:          |
| comment       | :x:                |
| skill6-16     | :x:                |
| class4-16     | :x:                |
| conversation  | :x:                |
| countsecret   | :heavy_check_mark: |
| arg0str       | :x:                |
| gravity       | :heavy_check_mark: |
| health        | :heavy_check_mark: |
| renderstyle   | :x:                |
| fillcolor     | :x:                |
| alpha         | :x:                |
| score         | :x:                |
| pitch         | :x:                |
| roll          | :x:                |
| scalex        | :x:                |
| scaley        | :x:                |
| scale         | :x:                |
| floatbobphase | :x:                |

#### Notes
- `skill1-2` and `skill4-5` are currently collapsed to the existing `easy` and `hard` flags.
- `arg*` currently has hexen's limit of 0-255.