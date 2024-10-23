#include "Wall.h"
#include <math.h>
#include "../Imageio/Imageio.h"

Intersect_Cond Wall::intersection_check(const M3DVector3f start, const M3DVector3f dir, float &distance, M3DVector3f intersection_p)
{
	if (_tr1.intersection_check(start,dir,distance,intersection_p) == _k_hit)
	{
		return _k_hit;
	}else
	{
		return _tr2.intersection_check(start,dir,distance,intersection_p);
	}
}


void	Wall::shade(M3DVector3f view, M3DVector3f intersect_p,const Light & sp_light, M3DVector3f am_light, M3DVector3f color,bool shadow)
{
	// Step 1: Ambient Lighting
    M3DVector3f Ambient;
    m3dLoadVector3(Ambient, 0.1, 0.1, 0.1);
    m3dScaleVector3(Ambient, _ka); // Scale ambient light by material's ambient coefficient (_ka)

	// Compute the surface normal
    M3DVector3f normal;
    _tr1.normal(normal); // Get normal from one of the triangles (I chose _tr1)
    printf("The wall normal is [%f, %f, %f]\n", normal[0], normal[1], normal[2]);
    m3dNormalizeVector(normal);
    printf("The wall normalized normal is [%f, %f, %f]\n", normal[0], normal[1], normal[2]);
    
    // Adjust the normal based on the view direction
    M3DVector3f view_dir;
    m3dSubtractVectors3(view_dir, view, intersect_p); // Calculate view direction (from intersection point to viewer)
    printf("The wall view_dir is [%f, %f, %f]\n", view_dir[0], view_dir[1], view_dir[2]);
    m3dNormalizeVector(view_dir); // Normalize the view direction
    printf("The wall normalized view_dir is [%f, %f, %f]\n", view_dir[0], view_dir[1], view_dir[2]);

    // Check if the normal is facing the viewer. If not, flip it.
    float dotProduct = m3dDotProduct(normal, view_dir);
    if (dotProduct < 0.0f) {
        m3dScaleVector3(normal, -1.0f); // Flip the normal
    }

	// Diffuse lighting
	M3DVector3f light_dir;
	M3DVector3f light_pos;
	M3DVector3f light_color;
	sp_light.get_light(light_pos, light_color);
    m3dSubtractVectors3(light_dir, light_pos, intersect_p); // Light direction (from intersection point to light)
    m3dNormalizeVector(light_dir); // Normalize the light direction

	float NdotL = m3dDotProduct(normal, light_dir); // Dot product of normal and light direction
    NdotL = fmax(NdotL, 0.0f); // Ensure non-negative (clamp to 0)

    M3DVector3f Diffuse;
    m3dCopyVector3(Diffuse, _color); // Copy the light color
    m3dScaleVector3(Diffuse, _kd * NdotL); // Scale by material's diffuse coefficient and NdotL

	//Specular Lighting
	//Calculate reflection vector R = 2 * (N dot L) * N - L
    M3DVector3f reflect_dir;
    float NL = m3dDotProduct(normal, light_dir);
    M3DVector3f scaled_normal;
    m3dCopyVector3(scaled_normal, normal);
    m3dScaleVector3(scaled_normal, 2.0f * NL);
    m3dSubtractVectors3(reflect_dir, scaled_normal, light_dir); // reflect_dir = 2*(N.L)*N - L
    m3dNormalizeVector(reflect_dir);

    // Normalize view direction (already passed as 'view') and reflection direction
    m3dNormalizeVector(view);
	m3dNormalizeVector(reflect_dir);

	//calculate shininess coefficient
    float cosAlpha = fmax(m3dDotProduct(view, reflect_dir), 0.0f);// Calculate the cosine of the angle between the view direction and the reflection direction (v dot r)^α 


    // Calculate specular intensity: ks * max( V . R, 0)^α
    float spec_intensity = pow(cosAlpha, _shininess);

	M3DVector3f Specular;
    m3dCopyVector3(Specular, _color); // Copy light color for specular
    m3dScaleVector3(Specular, _ks * spec_intensity); // Scale by material's specular coefficient and intensity

    // Combine ambient, diffuse, and specular components
    for (int i = 0; i < 3; i++) {
    color[i] = fminf(fmaxf(Ambient[i] + Diffuse[i] + Specular[i], 0.0f), 1.0f); // Final color is the sum of ambient, diffuse, and specular components
	}
}


void	Wall::get_reflect_direct(const M3DVector3f direct0,const M3DVector3f intersect_p, M3DVector3f reflect_direct)
{
	
}


void Wall::load_texture(std::string file_name)
{
	
}

void Wall::texture_color(M3DVector3f pos, M3DVector3f color)
{
}

void	Wall::get_texel(float x, float y, M3DVector3f color)
{
	return;
}