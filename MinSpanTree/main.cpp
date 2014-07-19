#include <iostream>
#include <vector>
#include "MSTGraph.h"
#include "MSTEdge.h"
#include "GLMinSpanTree.h"

#include "../ModelFitting/ModelSet.h"
#include "../ModelFitting/GLViwerModel.h"
#include "../ModelFitting/Neighbour26.h"

using namespace std;
using namespace MST;
using namespace cv;

/*Note: A lot of code from ModelFitting is being recompiled and reused here. */

int main()
{
    Mat temp = Mat(200, 200, CV_8UC3);
    cv::imshow( "", temp );

    ModelSet models;
    bool flag = models.deserialize( "data15_134_113_116" );
    if( !flag )
    {
        return 0;
    }


    // compute the projection point and put them into a vector
    vector<Vec3d> projs( models.tildaP.size() );
    for( unsigned i=0; i<models.tildaP.size(); i++ )
    {
        const int& lineid1 = models.labelID[i];
        const Line3D* line1   = models.lines[lineid1];
        const cv::Vec3i& pos1 = models.tildaP[i];
        projs[i] = line1->projection( pos1 );
    }


    Graph<Edge> graph( models.tildaP.size() );
    for( unsigned i=0; i<models.tildaP.size(); i++ )
    {
        for( int n=0; n<26; n++ )
        {
            const int& lineid1 = models.labelID[i];
            const Line3D* line1   = models.lines[lineid1];
            const cv::Vec3i& pos1 = models.tildaP[i];

            cv::Vec3i pos2;
            Neighbour26::getNeigbour( n, pos1, pos2 );
            if ( !models.labelID3d.isValid( pos2 ) ) continue;

            const int lineid2 = models.labelID3d.at( pos2 );
            if ( lineid2==-1 ) continue;

            const Line3D* line2   = models.lines[lineid2];

            const Vec3d proj1 = line1->projection( pos1 );
            const Vec3d proj2 = line2->projection( pos2 );

            const double dist = sqrt( proj1.dot( proj2 ) );

            // TODO: line id is equivalent to point id in this case
            graph.add_edge( Edge(lineid1, lineid2, dist) );
        }
    }
    Graph<Edge> tree( models.tildaP.size() );
    graph.get_min_span_tree( tree );

    Graph<Edge, cv::Vec3d> tree2;
    tree2.reset( projs, tree.get_edges() );



    GLViwerModel vis;

    GLViewer::GLLineModel *model = new GLViewer::GLLineModel( Vec3i(134,113,116) );
    model->updatePoints( models.tildaP );
    model->updateModel( models.lines, models.labelID );
    vis.objs.push_back( model );

    cout << models.labelID3d.get_size() << endl;

    GLViewer::GLMinSpanTree *mstobj = new GLViewer::GLMinSpanTree( tree2, models.labelID3d.get_size() );
    vis.objs.push_back( mstobj );

    vis.display(640, 480, 2);

    return 0;
}


int example( )
{
    // Yuchen: Minimum Spanning Tree

    /* Test Case Input
     [1] --3-- [2]
      |       / | \
      |     /   |   \
      7    2    4    6
      |  /      |      \
      |/        |        \
     [3] --1-- [4] --5-- [0]

     Result Output
     [1] --3-- [2]
              /
            /
           2
         /
       /
     [3] --1-- [4] --5-- [0] */

    //// Build Graph
    Graph<Edge> graph( 5 );
    graph.add_edge( Edge(1, 2, 3) );
    graph.add_edge( Edge(1, 3, 7) );
    graph.add_edge( Edge(2, 3, 2) );
    graph.add_edge( Edge(2, 4, 4) );
    graph.add_edge( Edge(2, 0, 6) );
    graph.add_edge( Edge(3, 4, 1) );
    graph.add_edge( Edge(4, 0, 5) );

    // Compute Minimum Spanning Tree
    Graph<Edge> tree( graph.num_nodes() );
    graph.get_min_span_tree( tree );

    //// Print Result
    cout << "Original Graph: " << graph << endl;
    cout << "Output MST Tree: "<< tree << endl;

    return true;
}
