#include <iostream>
#include <iomanip>
#include <fstream>
#include "astar.h"
#include "graph.h"
#include <algorithm>
#include <cmath>
#include <ctime>

struct EdgeTiger {
	EdgeTiger(size_t id1, size_t id2, float weight, std::string _name, std::vector< std::pair<float,float> > _points ) : id1(id1),id2(id2),weight(weight),name(_name),points(_points) {}
	EdgeTiger(size_t id1, size_t id2, float weight, std::string _name="") : id1(id1),id2(id2),weight(weight),name(_name) {}
	size_t GetID1() const { return id1; }
	size_t GetID2() const { return id2; }
	float  GetWeight() const { return weight; }
	std::string GetName() const { return name; }
	std::vector< std::pair<float,float> >  GetPoints() const { return points; }
	private:
	size_t id1;
	size_t id2;
	float weight; //miles
	std::string name; //street name 
	std::vector< std::pair<float,float> > points; //interminiate coordinates for drawing the edge
};

struct VertexTiger {
	VertexTiger (size_t _id, float _x=0.0f, float _y=0.0f, float _z = 0.0f, std::string _name="") : id(_id),x(_x),y(_y),z(_z),name(_name) 	{	}
	size_t ID() const { return id; }
	bool operator< ( const VertexTiger& rhs) const { return id<rhs.id; }
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }
	std::string GetName() const { return name; }
	private:
	size_t id;
	float x,y,z;
	std::string name; //location name - I defined only few - just to make output nicer
};

//heuristic to use when working with euclidian coordinates 
template <typename GraphType>
struct HeuristicEuclidian {
	typedef float ReturnType;
	ReturnType operator () (const GraphType & graph, 
			const typename GraphType::Vertex & v1,  
			const typename GraphType::Vertex & v2) const {
		float lat1 = v1.X();
		float lng1 = v1.Y();
		float lat2 = v2.X();
		float lng2 = v2.Y();
		std::cout << "Heuristic " << lat1 << " " << lng1 << " "
			<< lat2 << " " << lng2 << " "<< std::endl;
		return std::sqrt((lat1-lat2)*(lat1-lat2)+(lng1-lng2)*(lng1-lng2));
	}
};

//heuristic to use when working with geographical coordinates
//notice that this is a hack - see comment below
template <typename GraphType>
struct HeuristicKingLatLong {
	typedef float ReturnType;
	ReturnType operator () (const GraphType & graph, 
			const typename GraphType::Vertex & v1,  
			const typename GraphType::Vertex & v2) const {
		//using approximation
		//DEPENDS on latitude!!!!!!
		float hor_offset = 47.234f*(v1.X()-v2.X()); // 50.96 for NewYork
		float vert_offset = 68.98f*(v1.Y()-v2.Y());
		return std::sqrt(hor_offset*hor_offset+vert_offset*vert_offset);
	}
	//Here is the correct formula for distance between to points
	//given by latitude and lontiude, but it's very expensive
	//    theta = lon1 - lon2
	//    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta))
	//    dist = acos(dist)
	//    dist = rad2deg(dist)
	//    distance = dist * 60 * 1.1515
};

int main() try {
	//read vertices
	//read edges
	//call search
	//nicely display the result
	typedef Graph<VertexTiger,EdgeTiger> GraphTiger;
	GraphTiger graph;
	std::string vertex_file("king.county.vertexes.txt"); //"NY-vertex"
	std::string edge_file("king.county.edges.txt");//"NY-edge"
	std::ifstream inv(vertex_file.c_str());
	size_t num_vertices = 0;
	if (inv)  {
		std::string line;
		while ( !inv.eof() ) {
			std::getline(inv,line);
			//std::cout << line << std::endl;
			std::string::size_type pos = 0, prev_pos = 0;

			pos = line.find( ";", pos );
			if (pos==std::string::npos) break;
			size_t vertex_id = std::atoi( line.substr(prev_pos,pos).c_str() );

			prev_pos = pos+1;
			pos = line.find( ";", pos+1 );
			//std::cout << "LNG " << line.substr(prev_pos,pos-prev_pos) << std::endl;
			float lng = (float)std::atof( line.substr(prev_pos,pos-prev_pos).c_str() );

			prev_pos = pos+1;
			pos = line.find( ";", pos+1 );
			//std::cout << "LAT " << line.substr(prev_pos,pos-prev_pos) << std::endl;
			float lat = (float)std::atof( line.substr(prev_pos,pos-prev_pos).c_str() );

			std::string name = line.substr(pos+1);
			graph.InsertVertex( GraphTiger::Vertex(vertex_id,lng,lat,0.0f,name) );
			++num_vertices;
			//std::cout << "Vertex name " << name << std::endl;
		}
		std::ifstream ine(edge_file.c_str());
		size_t num_edges = 0;
		if (ine)  {
			std::string line;
			while ( !ine.eof() ) {
				std::getline(ine,line);
				//std::cout << line << std::endl;
				std::string::size_type pos = 0, prev_pos = 0;

				pos = line.find( ";", pos+1 );
				if (pos==std::string::npos) break;
				size_t edge_id = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t id1 = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t id2 = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				float length = (float)std::atof( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				std::string name = line.substr(prev_pos,pos-prev_pos);

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t zipcode = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t left_side_house_number_start = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );
				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t left_side_house_number_stop = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t right_side_house_number_start = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );
				prev_pos = pos+1;
				pos = line.find( ";", pos+1 );
				size_t right_side_house_number_stop = std::atoi( line.substr(prev_pos,pos-prev_pos).c_str() );

				//std::cout << "edge " << edge_id << " from " << id1 << " to " << id2 << " length " << length << std::endl;

				//array of points for drawing, ';' separated
				std::vector<std::pair<float,float> > points;
				std::string::size_type pos1=0,pos2=0;
				while ( 
						(pos1 = line.find( ";", pos+1 )) != std::string::npos 
						&&
						(pos2 = line.find( ";", pos1+1 )) != std::string::npos 
						) {
					float lng = (float)std::atof( line.substr(pos+1,pos1-pos-1).c_str() );
					//std::cout << "LONG " << line.substr(pos+1,pos1-pos-1).c_str() << std::endl;
					float lat = (float)std::atof( line.substr(pos1+1,pos2-pos1-1).c_str() );
					//std::cout << "LATI " << line.substr(pos1+1,pos2-pos1-1).c_str() << std::endl;
					points.push_back( std::make_pair<float,float>(lng,lat) );
					//std::cout << "point " << lat << " " << lng << std::endl;
					pos = pos2;
				}

				graph.InsertEdge( GraphTiger::Edge(id1,id2,length,name,points) );
				graph.InsertEdge( GraphTiger::Edge(id2,id1,length,name,points) );
				++num_edges;
			}

			std::cout << num_vertices << " vertices and " << num_edges << " edges\n";
			//graph.Print();

			Astar<GraphTiger,HeuristicKingLatLong<GraphTiger> > astar(graph);
			size_t start_id = 54576, goal_id=35986; //DP -> SeaTac
			//size_t start_id = 54576, goal_id=46451; //DP -> BelRed
			//size_t start_id = 54576, goal_id=67223; //DP -> Issaquah
			//size_t start_id = 54576, goal_id=54586; //DP -> some place about 2 miles down 148th
			clock_t start = std::clock();
			std::vector<GraphTiger::Edge> solution = astar.search(start_id,goal_id);
			clock_t finish = std::clock();
			std::cout << "Query took " << (finish-start) << std::endl;
			std::cout << "Solution" << std::endl;
			std::cout << "To get to " << graph.GetVertex(goal_id).GetName() 
				<< " from " << graph.GetVertex(start_id).GetName() << ":\n";
			size_t sol_size = solution.size();
			float cost = 0.0f,segment_cost=0.0f;
			std::string prev_street="no street";
			std::vector<GraphTiger::Edge>::reverse_iterator rit;
			for(rit=solution.rbegin();rit!=solution.rend();++rit) {
				cost += rit->GetWeight();
				if ( rit->GetName() == prev_street ) {
					segment_cost += rit->GetWeight();
				}
				else {
					if (segment_cost>0.0f) 
						std::cout << " (" << segment_cost << " miles) " << std::endl;
					std::cout << "\ttake " << rit->GetName();
					segment_cost = rit->GetWeight();
				}
				prev_street = rit->GetName();
			}
			std::cout << " (" << segment_cost << " miles) " << std::endl;
			std::cout << "total cost " << cost << std::endl;
			/*
			//write ALL points for KML file
			for(rit=solution.rbegin();rit!=solution.rend();++rit) {
				std::vector<std::pair<float,float> > points = rit->GetPoints();
				std::vector<std::pair<float,float> >::const_iterator it_points;
				for(it_points=points.begin();
						it_points!=points.end();
						++it_points) {
					std::cout << std::setprecision(18) << it_points->first << "," << it_points->second << ",0" << std::endl;

				}
			}
			*/
		}
		else {
			std::cerr << "cannot open file for reading" << std::endl;
		}
	}
}
catch (const char * msg) {
	std::cout << msg << std::endl;
}
