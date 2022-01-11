#include "render.h"

GLuint VaoId, VboId, EboId, ProgramId, viewLocation, projLocation, codColLocation, translate_location;


// variabile
int codCol;

// variabile pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float Obsx = -300.0, Obsy = 100, Obsz = 0;
float Vx = 0.0f, Vy = -1.0f, Vz = 0;
float incr_alpha1 = 0.01, incr_alpha2 = 0.01;
float znear = 1, fov = 30;
float alpha = 0, beta = 0;
std::vector<int> obj;


POINT last_cursor_pos;
bool first_frame = true;
// variabile pentru matricea de proiectie

// vectori
glm::vec3 Obs, PctRef, Vert;
std::vector <Block> blocks;
int click;
GLuint textures[150];
// matrice utilizate
glm::mat4 view, projection, translate;

GLuint LoadTexture(const char* filename)
{
	GLuint texName;
	//makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texName;
}


void handle_keyboard(unsigned char key, int x, int y) {
	switch (key) {
		key = tolower(key);
	case 'w':
		Obsx += MOVE_SPEED * cos(beta) * cos(alpha);
		Obsy += MOVE_SPEED * sin(alpha);
		Obsz += MOVE_SPEED * sin(beta) * cos(alpha);
		break;				 
	case 's':				 
		Obsx -= MOVE_SPEED * cos(beta) * cos(alpha);
		Obsy -= MOVE_SPEED * sin(alpha);
		Obsz -= MOVE_SPEED * sin(beta) * cos(alpha);
		break;				 
	case 'a':				 
		Obsz -= MOVE_SPEED * cos(beta);
		Obsx += MOVE_SPEED * sin(beta);
		break;				 
	case 'd':				 
		Obsz += MOVE_SPEED * cos(beta);
		Obsx -= MOVE_SPEED * sin(beta);
		break;
	}
}

void handle_mouse(int button, int state, int x, int y)
{
	if (state == 1) {
		click = button + 1;
	}
}

int hit_by_camera_ray() {
	int ret = -1;
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i].dist(Obsx, Obsy, Obsz) < RANGE && blocks[i].hit_by_ray(Obsx, Obsy, Obsz, cos(beta) * cos(alpha), sin(alpha), sin(beta) * cos(alpha))) {
			if (ret == -1) {
				ret = i;
			}
			else if(blocks[i].dist(Obsx, Obsy, Obsz) < blocks[ret].dist(Obsx, Obsy, Obsz)) {
				ret = i;
			}
		}
	}
	return ret;
}

void update_blocks() {
	if(click != 0) {
		int pos_block = hit_by_camera_ray();
		if (pos_block != -1) {
			if (click == 1) {
				blocks.erase(blocks.begin() + pos_block);
			}
			if(click == 3) {
				auto new_block = blocks[pos_block];
				int side = blocks[pos_block].hit_by_ray(Obsx, Obsy, Obsz, cos(beta) * cos(alpha), sin(alpha), sin(beta) * cos(alpha));
				if (side == 1) {
					new_block.x--;
				}
				if(side == 2) {
					new_block.x++;
				}
				if (side == 3) {
					new_block.y--;
				}
				if (side == 4) {
					new_block.y++;
				}
				if (side == 5) {
					new_block.z--;
				}
				if (side == 6) {
					new_block.z++;
				}
				blocks.push_back(new_block);
			}
		}
	}
}

void render_function() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	if (!first_frame) {
		int delta_x = cursor_pos.x - CENTER_X;
		int delta_y = cursor_pos.y - CENTER_Y;
		alpha -= delta_y * CHANGE_ANGLE_SPEED;
		beta += delta_x * CHANGE_ANGLE_SPEED;
		alpha = std::min(alpha, PI / 2 - 0.1f);
		alpha = std::max(alpha, -PI / 2 + 0.1f);
	}
	//std::cout << Obsx << " " << Obsy << " " << Obsz << "\n";
	//std::cout << alpha << " " << beta << "\n";
	first_frame = false;
	SetCursorPos(CENTER_X, CENTER_Y);
	// CreateVBO(); // decomentati acest rand daca este cazul 
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	update_blocks();
	click = 0;
	//pozitia observatorului
	
	//Refx = Obsx + 2 * real_cos(beta) * real_cos(alpha);
	//Refy = Obsy + 2 * real_cos(beta) * real_sin(alpha);
	//Refz = Obsz + 2 * real_sin(beta);
	Refx = Obsx + 2 * cos(beta) * cos(alpha);
	Refy = Obsy + 2 * sin(alpha);
	Refz = Obsz + 2 * sin(beta) * cos(alpha);
	// reperul de vizualizare 
	//std::cout << Refx << " " << Refy << " " << Refz << "\n";
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie, pot fi testate si alte variante
	projection = glm::infinitePerspective(fov, GLfloat(WIN_WIDTH) / GLfloat(WIN_HEIGHT), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	int pos = hit_by_camera_ray();
	//std::cout << pos << "\n";
	for (int i = 0; i < blocks.size(); i++) {
		auto b = blocks[i];
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(b.x * BLOCK_SIZE, b.y * BLOCK_SIZE, b.z * BLOCK_SIZE));
		glUniformMatrix4fv(translate_location, 1, GL_FALSE, &translate[0][0]);
		codCol = 0;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
		codCol = 1;
		glUniform1i(codColLocation, codCol);
		glLineWidth(1.5);
		if (i == pos) {
			glLineWidth(5);
			//std::cout << blocks[i].hit_by_ray(Obsx, Obsy, Obsz, cos(beta) * cos(alpha), sin(alpha), sin(beta) * cos(alpha)) << "\n";
		}
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40));
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44));
	}
	glutSwapBuffers();
	glFlush();
	Sleep(1000 / 60.);
}


void initiliaze_opengl() {
	glClearColor(135 / 255., 206 / 255., 235 / 255., 0.0);
	create_vbo();
	create_shaders();
	
	textures[0] = LoadTexture("dirt.png");

	// Locatii ptr shader
	viewLocation = glGetUniformLocation(ProgramId, "viewShader");
	projLocation = glGetUniformLocation(ProgramId, "projectionShader");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	translate_location = glGetUniformLocation(ProgramId, "translate_matrix");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void create_vbo(void)
{
	// varfurile 
	GLfloat Vertices[] =
	{		
		-BLOCK_SIZE / 2,  -BLOCK_SIZE / 2,  -BLOCK_SIZE / 2, 1.0f,  0.0f, .4f, 0.0f,
		 BLOCK_SIZE / 2,  -BLOCK_SIZE / 2,  -BLOCK_SIZE / 2, 1.0f,  0.0f, .4, 0.0f,
		 BLOCK_SIZE / 2,   BLOCK_SIZE / 2,  -BLOCK_SIZE / 2, 1.0f,   0.0f, .4f, 0.0f,
		-BLOCK_SIZE / 2,   BLOCK_SIZE / 2,  -BLOCK_SIZE / 2, 1.0f,   0.0f, .4f, 0.0f,
		-BLOCK_SIZE / 2,  -BLOCK_SIZE / 2,   BLOCK_SIZE / 2, 1.0f,   0.0f, .4f, 0.0f,
		 BLOCK_SIZE / 2,  -BLOCK_SIZE / 2,   BLOCK_SIZE / 2, 1.0f,   0.0f, .4f, 0.0f,
		 BLOCK_SIZE / 2,   BLOCK_SIZE / 2,   BLOCK_SIZE / 2, 1.0f,    0.0f, .4f, 0.0f,
		-BLOCK_SIZE / 2,   BLOCK_SIZE / 2,   BLOCK_SIZE / 2, 1.0f,    0.0f, .4f, 0.0f,
	};

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
		1, 0, 2,   2, 0, 3,  //  Fata "de jos"
		2, 3, 6,   6, 3, 7,  // Lateral 
		7, 3, 4,   4, 3, 0,  // Lateral 
		4, 0, 5,   5, 0, 1,  // Lateral 
		1, 2, 5,   5, 2, 6,  // Lateral 
		5, 6, 4,   4, 6, 7, //  Fata "de sus"
		0, 1, 2, 3,  // Contur fata de jos
		4, 5, 6, 7,  // Contur fata de sus
		0, 4, // Muchie laterala
		1, 5, // Muchie laterala
		2, 6, // Muchie laterala
		3, 7 // Muchie laterala
	};

	// generare VAO/buffere
	glGenBuffers(1, &VboId); // atribute
	glGenBuffers(1, &EboId); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // "copiere" in bufferul curent
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW); // "copiere" indici in bufferul curent

	// se activeaza lucrul cu atribute; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
}
void destroy_vbo(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);
}
void create_shaders(void)
{
	ProgramId = LoadShaders("08_02_Shader.vert", "08_02_Shader.frag");
	glUseProgram(ProgramId);
}
void destroy_shaders(void)
{
	glDeleteProgram(ProgramId);
}

void cleanup(void)
{
	destroy_shaders();
	destroy_vbo();
}

