#version 330

// Input vertex attributes from raylib
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Receive from code
uniform vec2 textureSize; 

// Output color
out vec4 finalColor;

vec2 uv_klems(vec2 uv, vec2 tex_size) 
{
    vec2 pixels = uv * tex_size + 0.5;
    
    vec2 fl = floor(pixels);
    vec2 fr = fract(pixels);
    vec2 aa = fwidth(pixels) * 0.75;

    fr = smoothstep(vec2(0.5) - aa, vec2(0.5) + aa, fr);
    
    return (fl + fr - 0.5) / tex_size;
}

void main()
{
    vec4 texel = texture(texture0, uv_klems(fragTexCoord, textureSize));
    
    finalColor = texel * fragColor * colDiffuse;
}