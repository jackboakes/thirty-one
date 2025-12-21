#version 330

in vec2 fragTexCoord;
in vec4 fragColour;

out vec4 finalColour;

uniform vec2 iResolution;
uniform float iTime;
uniform vec3 iColour; 

float hash(float x)
{
    return fract(21654.6512 * sin(385.51 * x));
}

float hash(vec2 p)
{
    return fract(21654.65155 * sin(35.51 * p.x + 45.51 * p.y));
}

float noise(vec2 p)
{
    vec2 fl = floor(p);
    vec2 fr = fract(p);

    fr.x = smoothstep(0.0, 1.0, fr.x);
    fr.y = smoothstep(0.0, 1.0, fr.y);

    float a = mix(hash(fl + vec2(0.0, 0.0)), hash(fl + vec2(1.0, 0.0)), fr.x);
    float b = mix(hash(fl + vec2(0.0, 1.0)), hash(fl + vec2(1.0, 1.0)), fr.x);

    return mix(a, b, fr.y);
}

float fbm(vec2 p)
{
    float v = 0.0, f = 1.0, a = 0.5;
    for(int i = 0; i < 5; i++)
    {
        v += noise(p * f) * a;
        f *= 2.0;
        a *= 0.5;
    }
    return v;
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    float time = iTime * 0.25;
    
    vec2 uv = fragCoord.xy / iResolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;    

    float p = fbm(vec2(noise(uv + time / 2.5), noise(uv * 2.0 + cos(time / 2.0) / 2.0)));

    vec3 col = pow(vec3(p), vec3(0.3)) - 0.4;
    col = mix(col, vec3(1.0), 1.0 - smoothstep(0.0, 0.2, pow(1.0 / 2.0, 0.5) - uv.y / 40.0));
    
    float s = smoothstep(.35, .6, col.x);
    float s2 = smoothstep(.47, .6, col.x);
    float s3 = smoothstep(.51, .6, col.x);
    

    vec3 cBase = col * iColour * s;
    vec3 cMid = (iColour * 0.5) * s2;
    vec3 cHot = vec3(1.0, 1.0, 1.0) * s3 * 0.8; 

    vec3 fireRGB = cBase + cMid + cHot;
    fireRGB *= 1.5; 
    
    vec3 bgRGB = (iColour * 0.2) + vec3(0.02);

    vec3 finalRGB = bgRGB + fireRGB;
    
    finalColour = vec4(finalRGB, 1.0);
}