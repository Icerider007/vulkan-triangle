#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

const int WIDTH = 640;
const int HEIGHT = 480;

//Start of math definitions
typedef struct{
	float data[4][4];	
}mat4;

typedef struct{
	float x,y,z,w;	
}vec4;

void mat4_identity(mat4* matrix){
	matrix->data[0][0] = 1;
	matrix->data[1][1] = 1;
	matrix->data[2][2] = 1;
	matrix->data[3][3] = 1;
}

vec4 vec4_create(float x, float y, float z, float w){
	vec4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;

}

vec4 mat4_mult_vec4(const mat4* matrix, const vec4* vector){
	vec4 result;
	result.x = matrix->data[0][0] * vector->x + matrix->data[0][1] * vector->y + matrix->data[0][2] * vector->z + matrix->data[0][3] * vector->w;
	result.y = matrix->data[1][0] * vector->x + matrix->data[1][1] * vector->y + matrix->data[1][2] * vector->z + matrix->data[1][3] * vector->w;
	result.z = matrix->data[2][0] * vector->x + matrix->data[2][1] * vector->y + matrix->data[2][2] * vector->z + matrix->data[2][3] * vector->w;
	result.w = matrix->data[3][0] * vector->x + matrix->data[3][1] * vector->y + matrix->data[3][2] * vector->z + matrix->data[3][3] * vector->w;
	return result;
}

//End of math definitions

//HelloTriangle definitions
typedef struct{
	SDL_Window *window;
	VkInstance instance;
	//Placeholder for vulkan vars
}HelloTriangleApp;

int run(HelloTriangleApp* app);
void initVulkan(HelloTriangleApp* app);
SDL_Window* initWindow();
int createInstance(HelloTriangleApp *app);
void mainLoop(HelloTriangleApp* app);
void cleanup(HelloTriangleApp* app);


//Runs on startup
int main(int argc, char **argv)
{
	//SDL boilerplate
	if (!SDL_SetAppMetadata("Eat my triangles","5","com.in.yourass")){
		printf("Error setting app metadata:%s\n",SDL_GetError());
		return 1;
	}

	if (!SDL_Init(SDL_INIT_VIDEO)){
		printf("SDLInitError:%s\n",SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("win", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE|SDL_WINDOW_VULKAN);
	if(!win){
		printf("SDLWindowCreateError:%s\n",SDL_GetError());
		SDL_Quit();
		return 1;
	}
	//End of SDL boilerplate

	HelloTriangleApp app = {};

	if((run(&app)) != 0){
		fprintf(stderr, "An error occured\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*Close window event check loop
}*/

int run(HelloTriangleApp* app){
	initVulkan(app);
	mainLoop(app);
	cleanup(app);
}

void initVulkan(HelloTriangleApp* app){
	if(createInstance(app)){
		printf("Couldn't create instance, some bullshit error showed up\n");
	}
	printf("Vulkan initialized\n");
}

int createInstance(HelloTriangleApp *app){
	//Vulkan boilerplate for appInfo
	VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Eat my Triangles",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};

	uint32_t count_instance_extensions = 0;
	const char * const* instance_extensions = SDL_Vulkan_GetInstanceExtensions(&count_instance_extensions);
	if(instance_extensions == NULL){
		fprintf(stderr, "Failed to get Vulkan extensions from SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	int count_extensions = count_instance_extensions + 1;
	const char **extensions = SDL_malloc(count_extensions * sizeof(const char*));
	extensions[0] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	SDL_memcpy(&extensions[1], instance_extensions, count_instance_extensions * sizeof(const char*));

	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = count_extensions,
		.ppEnabledExtensionNames = extensions,
		.enabledLayerCount = 0,
	};

	//Create instance
	if (vkCreateInstance(&createInfo, NULL, &app->instance) != VK_SUCCESS) {
		fprintf(stderr, "Failed to create Vulkan instance.\n");
		exit(EXIT_FAILURE);
	}
}

SDL_Window* initWindow(){

	//SDL boilerplate
	if (!SDL_SetAppMetadata("This is a triangle","5","com.in.yourass")){
		printf("Error setting app metadata:%s\n",SDL_GetError());
		return NULL;
	}

	if (!SDL_Init(SDL_INIT_VIDEO)){
		printf("SDLInitError:%s\n",SDL_GetError());
		return NULL;
	}

	SDL_Window *win = SDL_CreateWindow("win", WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if(!win){
		printf("SDLWindowCreateError:%s\n",SDL_GetError());
		SDL_Quit();
		return NULL;
	}
	return win;
	//End of SDL boilerplate
}

void mainLoop(HelloTriangleApp* app){

	printf("Entering main loop\n");
 
	int close_requested = 0;
	for(;!close_requested;)
	{
	  SDL_Event sdl_event;
	  while (SDL_PollEvent(&sdl_event))
	  { 
	    switch (sdl_event.type) 
	    {
	      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	      {
		close_requested = 1;
	      }
	      break;
	    }
	  } 
	}
}

void cleanup(HelloTriangleApp* app){
	printf("Cleaning up resources\n");
	//Vulkan cleanup
	
	//SDL cleanup
	SDL_DestroyWindow(app);
	SDL_Quit();
	return 0;
}
