#include <iostream>
#include <graphics.h>
#include <cmath>
using namespace std;

const double DT = 0.001;
const double g = 10;

struct Vector{
    double x;
    double y;

    Vector(){}

    Vector(double x_, double y_){
        x = x_;
        y = y_;
    }

    Vector operator * (double d){
        Vector v  = Vector (x*d, y*d);
        return v;
    }

    Vector operator + (Vector v){
        return Vector(x + v.x, y + v.y);

    }

    Vector operator - (Vector v){
        return Vector(x - v.x, y - v.y);
    }

    Vector operator / (double d){
        return Vector(x/d, y/d);
    }

    double vsize(){
        return sqrt(x*x + y*y);
    }

};

struct Node{
    double mass;
    Vector velocity;
    Vector position;
    bool fixed;

    Node(){}

    Node(double mass_, Vector velocity_, Vector position_,bool fixed_){
        mass = mass_;
        velocity = velocity_;
        position = position_;
        fixed = fixed_;
    }

    void update(){
        if(fixed == true)
            return;
        position = position + velocity*DT;

    }

    void draw(){
        circle(position.x, position.y,5);
    }


};

struct Spring{
    int k;
    double init_length;
    Node *node1, *node2;
    Vector C;

    Spring(){}

    Spring(double init_length_, int k_, Node* node1_, Node* node2_){
        k = k_;
        init_length = init_length_;
        node1 = node1_;
        node2 = node2_;
    }



    void update(){
        C = node2->position - node1->position;
        double x = C.vsize();
        Vector f = (C*k*(x-init_length))/x;
        Vector a1 = f/node1->mass;
        Vector a2 = f*-1/node2->mass;
        node1->velocity = node1->velocity + a1*DT;
        node2->velocity = node2->velocity + a2*DT;
    }

    void draw(){
        line(node1->position.x,node1->position.y,node2->position.x,node2->position.y);
    }


};

Node* nodes;
Spring* springs;

int nodes_count;
int springs_count;

void add_node(Vector position, double mass, Vector velocity, bool fixed){
    nodes_count++;
    nodes = (Node*)realloc(nodes, nodes_count*sizeof(Node));
    nodes[nodes_count - 1] = Node(mass,velocity,position,fixed);
}

void add_spring(double init_length, int k, Node *node1, Node *node2){
    springs_count++;
    springs = (Spring*)realloc(springs, springs_count*sizeof(Spring));
    springs[springs_count - 1] = Spring(init_length, k, node1, node2);

}

void apply_gravity(){
    for(int i = 0; i < nodes_count; i++){
        nodes[i].velocity = nodes[i].velocity + Vector(0,g*DT);
    }

}

void update(){
    for(int i = 0 ; i < nodes_count ; i++){
        nodes[i].update();
    }

    apply_gravity();

    for(int j = 0 ; j < springs_count ; j++){
        springs[j].update();
    }

}

void render(){
    cleardevice();
    for(int i = 0; i < nodes_count ; i++){
        nodes[i].draw();
    }

    for(int j = 0; j < springs_count ; j++){
        springs[j].draw();
    }
}

void init(){
    nodes = (Node*) calloc(0,sizeof(Node));
    springs = (Spring*) calloc(0,sizeof(Spring));
    add_node(Vector(100,100), 800, Vector(0,0), true);
    add_node(Vector(100,150), 800, Vector(0,0), false);
    add_node(Vector(100,200), 800, Vector(0,0), false);
    add_node(Vector(100,250), 800, Vector(10,0), false);

    add_node(Vector(200,100), 800, Vector(0,0), true);
    add_node(Vector(200,150), 800, Vector(0,0), false);
    add_node(Vector(200,200), 800, Vector(-10,0), false);

    add_node(Vector(300,100), 800, Vector(0,0), true);
    add_node(Vector(300,150), 800, Vector(10,0), false);

    add_spring(50, 20, &nodes[0], &nodes[1]);
    add_spring(50, 20, &nodes[1], &nodes[2]);
    add_spring(50, 20, &nodes[2], &nodes[3]);

    add_spring(50, 20, &nodes[4], &nodes[5]);
    add_spring(50, 20, &nodes[5], &nodes[6]);

    add_spring(50, 20, &nodes[7], &nodes[8]);

}


main()
{
    initwindow(800,600);
    init();
    while(true){
        update();
        render();
        delay(1);

    }
	getch();
}
