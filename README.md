# Skeletal Animation

A simple application of **Skeletal Animation** using C++(OpenGL)

Game "Bubble Bubble" with animated character "Veigar" from "League of Legends"

## Overview

- Used **Obsidian** and **lol2dae** to extract and convert data from "League of Legends" original file. 
- Designed a xml reader using C++ to read .dae file
- Used **Pre-OpenGL 3.0** to render and animate the object using extracted data. 

## Collada File(.dae)

### Structure

Collada file is a xml file that contains all the information about the object.

- `<library_effect>`
- `<library_gemoetries>`
- `<library_controllers>`
- `<library_animations>`
- `<library_visual_scenes>`

### 1. library_effect

contains emission, ambient, diffuse value of the object

```xml
<library_effects>
    <effect id="DefaultLambert">
        <profile_COMMON>
            <technique sid="Base">
                <lambert>
                    <emission>
                        <color>0.0 0.0 0.0 1.0</color>
                    </emission>
                    <ambient>
                        <color>0.0 0.0 0.0 1.0</color>
                    </ambient>
                    <diffuse>
                        <color>0.5 0.5 0.5 1.0</color>
                    </diffuse>
                </lambert>
            </technique>
        </profile_COMMON>
    </effect>
</library_effects>
```

### 2. library_geometries

Contains data of vertex, vertex normal, texture coordinate, and vertex indices for each triangle

(x, y, z) coordinate for 3396 vertex = 10188, (s, t) texture coordinate for 3396 vertex = 6792 

```xml
<library_geometries>
    <geometry id="Mesh" name="Mesh">
        <mesh>
            <source id="MeshPosition">
                <float_array id="MeshPositionArray" count="10188">
                    -78.9962 222.403 -2.04144 -58.1045 243.456 -2.04144 -83.6613 ... 
                </float_array>
            </source>
            <source id="MeshNormal">
                <float_array id="MeshNormalArray" count="10188">
                    -0.408869 0.815873 -0.408873 -0.408248 0.816497 -0.408248 -0.447214 ... 
                </float_array>
            </source>  
            <source id="MeshTexture">
                <float_array id="MeshTextureArray" count="6792">
                    0.787006 0.900465 0.707263 0.983457 0.804086 0.827411 ... 
                </float_array>
            </source>
            
            <triangles count="5520" material="DefaultMaterial">
                <p>
                    95 50 402 402 50 199 50 13 199 50 2 ...
                </p>
            </triangles>
        </mesh>
    </geometry>
</library_geometries>
```

### 3. library_controllers

- MeshSkinBindPoses: Inverse transform matrix from original position of each joint to root joint, 4x4 matrix represented as 1x16 array.

```xml
<source id="MeshSkinBindPoses">
    <float_array id="MeshSkinBindPosesArray" count="832">
        -1 -0 -0 -0 0 1 0 0 -0 -0 -1 -0 0 0 0 1 0.826456 0.538324 -0.164853 ... 
    </float_array>
</source>
```

- MeshSkinWeights: Weight values listed without particular order.

```xml
<source id="MeshSkinWeights">
    <float_array id="MeshSkinWeightsArray" count="3706">
        1 0.754536 0.245464 0.667392 0.332608 0.84557 0.15443 0.752939 
    </float_array>
</source>               
```

- vertex_weights: Number of weight data for each vertex, and <joint index, weight index> pair data.

```xml
<vertex_weights count="3396">
    <input semantic="JOINT" source="#MeshSkinJoints" offset="0"></input>
    <input semantic="WEIGHT" source="#MeshSkinWeights" offset="1"></input>
    <vcount>
        1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
    </vcount>
    <v>
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
    </v>
</vertex_weights>
```

### 4. library_animations

Contains rotation, translation, scale, time of every keyframe for each joints, and interpolation type of the keyframes.

```xml
<library_animations>
    <animation>
        <animation>
            <!-- time -->
            <source id="Bit_ModelTranslationInput">
                <float_array id="Bit_ModelTranslationInputArray" count="188">
                    0 0.0333333 0.0666667 0.1 0.133333 0.166667 0.2 0.233333 0.266667 ...
                </float_array>
            </source>
            <!-- interpolation type -->
            <source id="Bit_ModelTranslationInterpolation">
                <Name_array id="Bit_ModelTranslationInterpolationArray" count="188">
                    LINEAR LINEAR LINEAR LINEAR LINEAR LINEAR LINEAR ...
                </Name_array>
            </source>
            <!-- translation, (x,y,z) coordinate -->
            <source id="Bit_ModelTranslation">
                <float_array id="Bit_ModelTranslationArray" count="564">
                    0 0 -0 0 0 -0 0 0 -0 0 0 -0 0 0 -0 0 0 -0 0 0 ...
            <!-- rotation, scale -->
                        ...
```

### 5. library_visual_scenes

Contains hierarchy information of joints

```xml
<node id="L_Hip" name="L_Hip" sid="L_Hip" type="JOINT">
    <translate sid="translate">-3.48503 -0.899708 0</translate>
    <rotate sid ="rotateX">1 0 0 9.57016</rotate>
    <rotate sid="rotateY">0 1 0 32.431</rotate>
    <rotate sid ="rotateZ">0 0 1 38.9299</rotate>
    <scale sid="scale">1 1 1</scale>
    <node id="L_KneeUpper" name="L_KneeUpper" sid="L_KneeUpper" type="JOINT">
        <translate sid="translate">-5.94371 -1.90735e-006 0</translate>
        <rotate sid ="rotateX">1 0 0 -1.38994e-006</rotate>
        <rotate sid="rotateY">0 1 0 -1.52514e-006</rotate>
        <rotate sid ="rotateZ">0 0 1 36.5953</rotate>
        <scale sid="scale">1 1 1</scale>
        <node id="L_KneeLower" name="L_KneeLower" sid="L_KneeLower" type="JOINT">
            <translate sid="translate">9.53674e-007 0 0</translate>
            <rotate sid ="rotateX">1 0 0 2.56132e-006</rotate>
            <rotate sid="rotateY">0 1 0 -1.06722e-007</rotate>
            <rotate sid ="rotateZ">0 0 1 2.78299e-014</rotate>
            <scale sid="scale">1 1 1</scale>
        </node>
    </node>
</node>
```

## Game Features

- Animated "Veigar" on the left side, "Bubble Bubble" on the right side
- basic rules of "Bubble Bubble" are implemented
- `space` shoots bubble, `esc` pauses game
- scoreboard keeps highest scores

## Screenshot

<p align="center">
    <img src="./images/screenshot.gif">
</p>

## Reference

[OpenGL Skeletal Animation](https://www.youtube.com/watch?v=f3Cr8Yx3GGA&t=572s)

