#include <nds.h>
#include <stdio.h>

//our very simple sprite engine...
#define  SPRITE_MAX 128

//these are placed in an array to allow for random size selection
//these are defined as part of the new sprite api
SpriteSize sizes[] = {
	SpriteSize_8x8, 
	SpriteSize_8x16,
	SpriteSize_16x8,
	SpriteSize_8x32,
	SpriteSize_16x16, 
	SpriteSize_32x8,
	SpriteSize_16x32,  
	SpriteSize_32x16, 
	SpriteSize_32x32, 
	SpriteSize_32x64, 
	SpriteSize_64x32, 
	SpriteSize_64x64, 	
};

//this is our game entity. Notice it has a bit more info than
//would fit into OAM.  This method is a lot more flexible than trying
//to treat oam as a game object directly.
typedef struct {
	int x,y,z;
	int dx, dy;
	bool alive;
	u16* gfx;
	SpriteColorFormat format;
	SpriteSize size;
}mySprite;

//an array of sprites
mySprite sprites[SPRITE_MAX];

u32 spriteMemoryUsage = 0;

u32 oomCount = 0;
u32 allocationCount = 0;
u32 spriteMemSize = 128 * 1024;

bool oom = false;
OamState *oam = &oamMain;

//a sprite constructor
void createSprite(mySprite* s, int x, int y, int z, SpriteSize size, SpriteColorFormat format, int dx, int dy) {
	s->alive = true;
	s->x = x;
	s->y = y;
	s->z = z; 
	s->dx = dx;
	s->dy = dy;
	s->size = size;
	s->format = format;
    
	//api: allocate a chunk of sprite graphics memory
	s->gfx = oamAllocateGfx(oam, size, format);
	
	allocationCount++;
	if(s->gfx) {
		spriteMemoryUsage += (size & 0xFFF) << 5;
		oom = false;
	} else {
		oom = true;
		//only a failure of the allocator if there was enough room
		if(spriteMemoryUsage + ((size & 0xFFF) << 5) < spriteMemSize)
			oomCount++;
	}
}

//sprite deconstructor
void killSprite(mySprite *s) {
	s->alive = false;  
 
	//api: free the graphics
	if(s->gfx) {	
		oamFreeGfx(oam, s->gfx);
		spriteMemoryUsage -= (s->size & 0xFFF) << 5;
	}

	s->gfx = 0;
}

//a qsort function which sorts on z order
int zsort(const void* a, const void* b) {
	mySprite *first = (mySprite*)a;
	mySprite *second = (mySprite*)b;

	//the trivial case 
	if(first == second) return 0;

	//handle nulls
	if(!first && second) return -1;
	if(first && !second) return 1;

	//a dead sprite is always after a living one in the sort
	if(!first->alive && second->alive) return -1;
	if(first->alive && !second->alive) return 1;
	if(!first->alive && !second->alive) return 0;

	//finally if both are alive and not null sort them by depth
	if(first->z == second->z) return 0;
	if(first->z < second->z ) return -1;
	if(first->z > second->z) return 1;

	return 0; 
}  
  
//map our sprite to oam entries
void updateSprites(void) {
	int i;

	//sort our sprites on z
	//a more efficient way would be to keep a sorted list of sprites
	qsort(sprites, SPRITE_MAX, sizeof(mySprite), zsort);

	//set oam to values required by my sprite
	for(i = 0; i < SPRITE_MAX; i++) {
		//an api function: void oamSet(OamState* oam, int id,  int x, int y, int priority, int palette_alpha, SpriteSize size, SpriteColorFormat format, const void* gfxOffset, int affineIndex, bool sizeDouble, bool hide);
		oamSet(oam, 
			i, 
			sprites[i].x, sprites[i].y, 
			0, 
			0,
			sprites[i].size,
			sprites[i].format, 
			sprites[i].gfx, 
			-1, 
			false, 
			!sprites[i].alive,
			false,
			false, 
			false);
	}
}

//create a sprite with a random position, speed, and size 
void randomSprite(mySprite* s) {
	//pick a random color index 
	u8 c = rand() % 256;

	//two pixels at a time
	u16 color = c | (c << 8);

	//create a randomly oriented sprite going off in a random direction
	createSprite(s, rand() % 256, rand() % 192, 0, sizes[(rand() % 12)], SpriteColorFormat_256Color, rand() % 4 - 2, rand() % 4 - 2);

	//dont let sprites get stuck with 0 velocity
	if(s->dx == 0 && s->dy == 0) {   
		s->dx = rand() % 3 + 1;
		s->dy = rand() % 3 + 1;
	}

	//the size (in pixels) is encoded in the low 12 bits of the Size attribute (shifted left by 5)
	//we load new graphics each time as this is as much a test of my allocator as an example of api usage
	if(s->gfx) {
		swiCopy(&color, s->gfx, ((s->size & 0xFFF) << 4) | COPY_MODE_FILL);
	}	else {	
		s->alive = false;
	}
}

//move the sprite based on its velocity, kill it if it goes off screen and create a new random one
void moveSprites(void) {
	int i;

	for(i = 0; i < SPRITE_MAX; i++) {
		sprites[i].x += sprites[i].dx;
		sprites[i].y += sprites[i].dy;

		if(sprites[i].x >= 256 || sprites[i].x < 0 || sprites[i].y >= 192 || sprites[i].y < 0) {
			killSprite(&sprites[i]);
			randomSprite(&sprites[i]);
		}
	}
}

int main(void)  {
	int i;
	int memUsageTemp = 0xFFFFFFFF;

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	consoleDemoInit();
//	consoleDebugInit(DebugDevice_NOCASH); //send stderr to no$gba debug window

	//api: initialize OAM to 1D mapping with XX byte offsets and no external palette
	oamInit(oam, SpriteMapping_1D_128, false);

	//create some sprites
	for(i = 0; i < SPRITE_MAX; i++)
		randomSprite(&sprites[i]);

	//load a randomly colored palette
	for(i = 0; i < 256; i++) {
      SPRITE_PALETTE[i] = rand();
      SPRITE_PALETTE_SUB[i] = rand();
	}

	while(1) { 
		moveSprites();

		updateSprites();

		swiWaitForVBlank();
		
		//api: updates real oam memory 
		oamUpdate(oam);

		if(oom) {	
			memUsageTemp = memUsageTemp > spriteMemoryUsage ? spriteMemoryUsage : memUsageTemp;
    	}	

		consoleClear();
		
		printf("Memory usage: %i %i%% \n",  spriteMemoryUsage, 100 * spriteMemoryUsage / (spriteMemSize));
		printf("Percentage fail: %i%% \n", oomCount * 100 / allocationCount);
		printf("Lowest Usage at fail %i %i%% \n", memUsageTemp, 100 * memUsageTemp / (spriteMemSize));				
	}

	return 0;
}
