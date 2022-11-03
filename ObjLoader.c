#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

double axisY = 0; 
double axisX = 0;

GLuint obj;

typedef struct vertex{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}Vertex;

typedef struct normal{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}Normal;

typedef struct texture{
    GLfloat x;
    GLfloat y;
}Texture;

typedef struct face{
    GLint v1, v2, v3;
    GLint n1, n2, n3;
    GLint t1, t2, t3;
}Face;

void DrawObj(FILE *fp){
    obj = glGenLists(1);

    char ch;
    int i;
    int res;

    Vertex *vertex = (Vertex *)malloc(1 * sizeof(Vertex));
    int Ivertex = 0;
    
    Normal *normal = (Normal *)malloc(1 * sizeof(Normal));
    int Inormal = 0;

    Texture *texture = (Texture *)malloc(1 * sizeof(Texture));
    int Itexture = 0;

    Face *face = (Face *)malloc(1 * sizeof(Face));
    int Iface = 0;
    
    glColor3f( 0.8, 0.0, 0.4 );
    glNewList(obj, GL_COMPILE);
    while(1){
        char lineHeader[128];
        res = fscanf(fp, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0){
            fscanf(fp, " %f %f %f", &vertex[Ivertex].x, &vertex[Ivertex].y, &vertex[Ivertex].z);

            Ivertex++;
            vertex = (Vertex *)realloc(vertex, (Ivertex + 1) * sizeof(Vertex));

        }else if (strcmp(lineHeader, "vn") == 0){
            fscanf(fp, " %f %f %f", &normal[Inormal].x, &normal[Inormal].y, &normal[Inormal].z);

            Inormal++;
            normal = (Normal *)realloc(normal, (Inormal + 1) * sizeof(Normal));

        }else if(strcmp(lineHeader, "vt") == 0){
            fscanf(fp, " %f %f", &texture[Itexture].x, &texture[Itexture].y);
            
            Itexture++;
            texture = (Texture *)realloc(texture, (Itexture+1) * sizeof(Texture ));

        }else if(strcmp(lineHeader, "f") == 0){
            fscanf(fp, " %d/%d/%d %d/%d/%d %d/%d/%d", &face[Iface].v1,&face[Iface].t1,&face[Iface].n1,&face[Iface].v2,&face[Iface].t2,&face[Iface].n2,&face[Iface].v3,&face[Iface].t3,&face[Iface].n3);
            
            Iface++;
            face = (Face *)realloc(face, (Iface+1) * sizeof(Face));
            //printf(" %d/%d/%d %d/%d/%d %d/%d/%d\n", face[Iface - 1].v1,face[Iface - 1].t1,face[Iface - 1].n1,face[Iface - 1].v2,face[Iface - 1].t2,face[Iface - 1].n2,face[Iface - 1].v3,face[Iface - 1].t3,face[Iface - 1].n3);
        }
    }
    
    
    glBegin(GL_TRIANGLES);
    for (i = 0; i < Iface - 1; i++){
        
        glVertex3f(vertex[face[i].v1 -1].x, vertex[face[i].v1 -1].y, vertex[face[i].v1 -1].z);
        glNormal3f(normal[face[i].n1 -1].x, normal[face[i].n1 -1].y, normal[face[i].n1 -1].z);
        glTexCoord2f(texture[face[i].t1 - 1].x, texture[face[i].t1 - 1].y);

        glVertex3f(vertex[face[i].v2 -1].x, vertex[face[i].v2 -1].y, vertex[face[i].v2 -1].z);
        glNormal3f(normal[face[i].n2 -1].x, normal[face[i].n2 -1].y, normal[face[i].n2 -1].z);
        glTexCoord2f(texture[face[i].t2 -1 ].x, texture[face[i].t2 -1 ].y);

        glVertex3f(vertex[face[i].v3 - 1].x, vertex[face[i].v3 - 1].y, vertex[face[i].v3 - 1].z);
        glNormal3f(normal[face[i].n3 - 1].x, normal[face[i].n3 - 1].y, normal[face[i].n3 - 1].z);
        glTexCoord2f(texture[face[i].t3 - 1].x, texture[face[i].t3 - 1].y);
    }
    glEnd();
    
    // glBegin(GL_POINTS);
    // for (i = 0; i < Ivertex; i++){
    //     glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
    // }
    // glEnd();
    
    
    glEndList();
    

    free(vertex);
    free(normal);
    free(texture);
    free(face);
}

void Draw() {   
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glRotatef( axisX, 1.0, 0.0, 0.0 );
    glRotatef( axisY, 0.0, 1.0, 0.0 );
    glCallList(obj);
    glPopMatrix();
}

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw();
	glutSwapBuffers();
}

void WindowSize(GLsizei w, GLsizei h)
{
	if(h == 0) h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D (-50.0f, 50.0f, -50.0f*h/w, 50.0f*h/w);
	else
		gluOrtho2D (-50.0f*w/h, 50.0f*w/h, -50.0f, 50.0f);
}

void Keyboard (unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    else if (key == 'd'){
        axisY += 5;
    }

    else if (key == 'a'){
        axisY -= 5;
    }

    else if (key == 'w'){
        axisX += 5;
    }

    else if (key == 's'){
        axisX -= 5;
    }


    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if (state == 1)
        return;
    if (button == 3)
        glScalef(1.1,1.1,1.1);
    else if (button == 4)
        glScalef(0.9, 0.9, 0.9);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    char *path = (char *)malloc(128 * sizeof(char));
    puts("Name/Paht of the .obj file: ");
    scanf(" %s", path);
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        printf("Impossible to open the file!\n");
        return 0;
    }

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(800, 450);
	glutInitWindowPosition(0, 0);
    glutCreateWindow(".obj file importation");
    
    glEnable(GL_CULL_FACE);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(mouse);
    
    DrawObj(fp);
    fclose(fp);

    glutMainLoop();

    return 0;
}
