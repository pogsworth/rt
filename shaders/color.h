#ifndef COLOR_H
#define COLOR_H

struct RGBAf{
	RGBAf() {}
	RGBAf( float _r, float _g, float _b, float _a ) { r=_r; g=_g; b=_b; a=_a; }
	float r,g,b,a;

	void operator-=(RGBAf& c) { r-=c.r; g-=c.g; b-=c.b; a-=c.a; }
	void operator+=(RGBAf& c) { r+=c.r; g+=c.g; b+=c.b; a+=c.a; }
	void operator*=(float s) { r*=s; g*=s; b*=s; a*=s; }
	RGBAf operator*(float s) { return RGBAf(r*s, g*s, b*s, a*s); }

	friend RGBAf operator*(float s, RGBAf rgba);
};

inline RGBAf operator*(float s, RGBAf rgba)
{
	return rgba.operator*(s);
}

inline RGBAf operator+(const RGBAf c, const RGBAf d)
{
	return RGBAf( c.r+d.r, c.g+d.g, c.b+d.b, c.a+d.a );
}

inline RGBAf operator-(const RGBAf c, const RGBAf d)
{
	return RGBAf( c.r-d.r, c.g-d.g, c.b-d.b, c.a-d.a );
}

struct MATERIAL
{
	RGBAf Ambient;
	RGBAf Diffuse;
	RGBAf Specular;
	RGBAf Emissive;
	float Power;
};

inline float clamp(float input)
{
	if (input<0.f)
		return 0.f;
	if (input>1.f)
		return 1.f;
	return input;
}

//					0-360			0-1				0-1
inline RGBAf hls2rgb(float hue, float lightness, float saturation)
{
	RGBAf rgb;

	if (hue < 0.f || hue > 360.f) {
		hue=fmodf(hue,360.f);
		if (hue < 0.f)
			hue+=360.f;
	}
	hue/=60.f;
	rgb.r=clamp(fabsf(hue - 3.f) - 1.f);
	rgb.g=clamp(2.f - fabsf(hue - 2.f));
	rgb.b=clamp(2.f - fabsf(hue - 4.f));

	rgb -= RGBAf(1.f,1.f,1.f,0.f);
	rgb *= saturation;
	rgb += RGBAf(1.f,1.f,1.f,0.f);
	rgb *= lightness;
	rgb.a=1.f;
	return rgb;
}

//return a component-wise multiply of the colors
inline RGBAf operator * (RGBAf& x, RGBAf& y)
{
    return RGBAf(x.r * y.r, x.g * y.g, x.b * y.b, x.a * y.a);
}

#endif // COLOR_H