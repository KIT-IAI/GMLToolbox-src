#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <map>

#include "tinyxml/tinyxml.h"

#include "Mesh.hpp"
#include "MeshLibrary.hpp"
#include "MaterialLibrary.hpp"
#include "Scene.hpp"
#include "Material.hpp"
#include "Node.hpp"
#include "Triangles.hpp"
#include "Polygons.hpp"
#include "ImageLibrary.hpp"
#include "Image.hpp"
#include "NCName.hpp"
#include "wrapio.hpp"
#include "NodeLibrary.hpp"
#include "all.hpp"


#include "Writer.hpp"

using namespace boost::property_tree;

namespace col
{
	const std::string XMLNS = "http://www.collada.org/2005/11/COLLADASchema";
	const std::string VERSION = "1.4.1";
    const std::string SCHEMA_LOCATION = "http://www.khronos.org/files/collada_schema_1_4";

    template <typename T>
    std::string complexJoin(const std::vector<T>& container, const std::string& delimiter)
    {
        std::stringstream temp;

        size_t i = 1;

        for (std::vector<T>::const_iterator it = container.begin(); it != container.end(); ++it)
        {
            if (it != container.begin())
            {
                temp << delimiter;   
            }

            temp << boost::lexical_cast<std::string>(*it);

            if ((i % 10) == 0)
            {
                //temp << std::endl;
            }

            ++i;
        }

        return temp.str();
    }

    std::string createGeometryId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-geometry";
    }

    std::string createPositionsArrayId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-positions-array";
    }

    std::string createNormalsArrayId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-normals-array";
    }

    std::string createTextureCoordinatesArrayId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-textureCoordinates-array";
    }

    std::string createPositionsId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-positions";
    }

    std::string createNormalsId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-normals";
    }

    std::string createTextureCoordinatesId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-textureCoordinates";
    }

    std::string createVerticesId(const MeshPtr& mesh)
    {
        return mesh->getId() + "-vertices";
    }

    std::string createTextureSamplerId(const TexturePtr& texture)
    {
        return texture->getId() + "-sampler";
    }

    std::string createTextureSurfaceId(const TexturePtr& texture)
    {
        return texture->getId() + "-surface";
    }
    
    std::string createMaterialReference(const MaterialPtr& material)
    {
        return material->getId() + "-ref";
    }

    std::string createMaterialReference(const PrimitivePtr& primitive)
    {
        MaterialPtr material = primitive->getMaterial().lock();
        return createMaterialReference(material);
    }

    

    void writeH(const Indices& indices, TiXmlElement* parentXml)
    {
        assert(indices.size() >= 3);

        TiXmlElement* hXml = new TiXmlElement("h");
        parentXml->LinkEndChild(hXml);

        std::string hStr = complexJoin(indices, " ");

        TiXmlText* hText = new TiXmlText(hStr);
        hXml->LinkEndChild(hText);
    }

    void writeP(const Indices& indices, TiXmlElement* trianglesXml)
    {
        assert(indices.size() >= 3);

        TiXmlElement* pXml = new TiXmlElement("p");
        trianglesXml->LinkEndChild(pXml);

        std::string pStr = complexJoin(indices, " ");

        TiXmlText* pText = new TiXmlText(pStr);
        pXml->LinkEndChild(pText);
    }

    std::string createDateTime()
    {
        using namespace boost::local_time;

        std::stringstream temp;
        time_zone_ptr tz;
        local_date_time now = local_sec_clock().local_time(tz);
        local_time_facet* o = new local_time_facet();
        temp.imbue(std::locale(std::locale::classic(), o));
        o->format("%Y-%m-%dT%H:%M:%S%Q");
        temp << now;

        return temp.str();
    }

	void writeAsset(TiXmlElement* colladaXml) 
	{
		TiXmlElement* assetXml = new TiXmlElement("asset");
		colladaXml->LinkEndChild(assetXml);

		// 2006-08-23T22:29:59Z
		TiXmlElement* createXml = new TiXmlElement("created");
		assetXml->LinkEndChild(createXml);

		{
			TiXmlText* text = new TiXmlText(createDateTime());
			createXml->LinkEndChild(text);
		}

		TiXmlElement* modifiedXml = new TiXmlElement("modified");
		assetXml->LinkEndChild(modifiedXml);

		{
			TiXmlText* text = new TiXmlText(createDateTime());
			modifiedXml->LinkEndChild(text);
		}

        // <up_axis>Z_UP</up_axis>
        TiXmlElement* upAxisXml = new TiXmlElement("up_axis");
        assetXml->LinkEndChild(upAxisXml);
        TiXmlText* xmlText = new TiXmlText("Z_UP");
        upAxisXml->LinkEndChild(xmlText);
	}

    void writeInput(const MeshPtr& mesh, TiXmlElement* parentXml)
    {
        TiXmlElement* inputXml = new TiXmlElement("input");
        parentXml->LinkEndChild(inputXml);

        inputXml->SetAttribute("offset", "0");
        inputXml->SetAttribute("semantic", "VERTEX");
        
        std::string sourceStr = '#' + createVerticesId(mesh);
        inputXml->SetAttribute("source", sourceStr);
    }

    void writePH(const PolygonsPtr& polygons, TiXmlElement* polygonsXml)
    {
        TiXmlElement* phXml = new TiXmlElement("ph");
        polygonsXml->LinkEndChild(phXml);

        writeP(polygons->getIndices(), phXml);

        const Holes& holes = polygons->getHoles();
        for (Holes::const_iterator it = holes.begin(); it != holes.end(); ++it)
        {
            writeH(*it, phXml);
        }
    }

    void writePolygons(const Primitives& primitives, const MeshPtr& mesh, TiXmlElement* meshXml)
    {
        // TODO: Group polygons by material
        typedef boost::unordered_map<MaterialPtr, Primitives> OrderedPrimitives;
        OrderedPrimitives orderedPrimitives;

        for (Primitives::const_iterator it = primitives.begin(); it != primitives.end(); ++it)
        {
            PrimitivePtr primitive = *it;
            MaterialPtr material = primitive->getMaterial().lock();

            OrderedPrimitives::iterator it2 = orderedPrimitives.find(material);

            if (it2 == orderedPrimitives.end())
            {
                Primitives temp;
                temp.push_back(primitive);

                orderedPrimitives.emplace(material, temp);
            }
            else
            {
                it2->second.push_back(primitive);
            }
        }

        for (OrderedPrimitives::const_iterator it = orderedPrimitives.begin(); it != orderedPrimitives.end(); ++it)
        {
            const Primitives& primitives = it->second;

            TiXmlElement* polygonsXml;
            
            if (primitives.front()->getType() == col::TRIANGLES)
            {
                polygonsXml = new TiXmlElement("triangles");
            }
            else
            {
                polygonsXml = new TiXmlElement("polygons");
            }
            meshXml->LinkEndChild(polygonsXml);

            if (it->first.use_count() > 0)
            {
                polygonsXml->SetAttribute("material", createMaterialReference(it->first));
            }

            writeInput(mesh, polygonsXml);

            Indices moreIndices;

            for (Primitives::const_iterator it = primitives.begin(); it != primitives.end(); ++it)
            {
                PrimitivePtr primitive = *it;

                if (primitive->getType() == POLYGONS)
                {
                    PolygonsPtr polygons = boost::static_pointer_cast<Polygons>(primitive);

                    if (polygons->hasHoles())
                    {
                        writePH(polygons, polygonsXml);
                    }
                    else
                    {
                        writeP(polygons->getIndices(), polygonsXml);
                    }
                }
                else
                {
                    const Indices& indices = primitive->getIndices();
                    std::copy(indices.begin(), indices.end(), std::back_inserter(moreIndices));
                }                
            }

            if (!moreIndices.empty())
            {
                writeP(moreIndices, polygonsXml);

                std::string count = boost::lexical_cast<std::string>(moreIndices.size() / 3);
                polygonsXml->SetAttribute("count", count);
            }
            else
            {
                std::string count = boost::lexical_cast<std::string>(primitives.size());
                polygonsXml->SetAttribute("count", count);
            }

            
        }
    }

    void writeFloatArray(const std::string& id, const std::vector<double>& floatArray, TiXmlElement* sourceXml)
	{
		TiXmlElement* floatArrayXml = new TiXmlElement("float_array");
		sourceXml->LinkEndChild(floatArrayXml);

		floatArrayXml->SetAttribute("id", id);

		std::string count = boost::lexical_cast<std::string>(floatArray.size());
		floatArrayXml->SetAttribute("count", count);

        std::string floatArrayStr = complexJoin(floatArray, " ");

		TiXmlText* floatArrayText = new TiXmlText(floatArrayStr);
		floatArrayXml->LinkEndChild(floatArrayText);
	}

    template <typename Op>
    std::vector<double> join(const Vertices& vertices, Op op)
    {
        std::vector<double> res;

        for (Vertices::const_iterator it = vertices.begin(); it != vertices.end(); ++it)        
        {
            op(*it, res);
        }

        return res;
    }

    void joinPosition(const Vertex& vertex, std::vector<double>& res)
    {
        Vector3 vec = vertex.getPosition();
        
        res.push_back(vec.getX());
        res.push_back(vec.getY());
        res.push_back(vec.getZ());
	}

    void joinNormal(const Vertex& vertex, std::vector<double>& res)
    {
        Vector3 vec = vertex.getNormal();
        res.push_back(vec.getX());
        res.push_back(vec.getY());
        res.push_back(vec.getZ());
    }

    void joinTextureCoordinate(const Vertex& vertex, std::vector<double>& res)
    {
        if (vertex.getTextureCoordinates())
        {
            Vector2 vec = *vertex.getTextureCoordinates();
            res.push_back(vec.getX());
            res.push_back(vec.getY());
        }
        else
        {
            res.push_back(0.0);
            res.push_back(0.0);
        }
    }

	void writeDefaultPositionTechnique(const MeshPtr& mesh, TiXmlElement* sourceXml)
	{
		const size_t STRIDE = 3; // X, Y, Z
		size_t count = mesh->getVertices().size();

		TiXmlElement* techniqueCommonXml = new TiXmlElement("technique_common");
		sourceXml->LinkEndChild(techniqueCommonXml);

		TiXmlElement* accessorXml = new TiXmlElement("accessor");
		techniqueCommonXml->LinkEndChild(accessorXml);

        accessorXml->SetAttribute("count", boost::lexical_cast<std::string>(count));
		accessorXml->SetAttribute("stride", 3);
		accessorXml->SetAttribute("source", '#' + createPositionsArrayId(mesh));

		TiXmlElement* paramXml = new TiXmlElement("param");
		accessorXml->LinkEndChild(paramXml);
		paramXml->SetAttribute("name", "X");
		paramXml->SetAttribute("type", "float");
		paramXml = new TiXmlElement("param");
		accessorXml->LinkEndChild(paramXml);
		paramXml->SetAttribute("name", "Y");
		paramXml->SetAttribute("type", "float");
		paramXml = new TiXmlElement("param");
		accessorXml->LinkEndChild(paramXml);
		paramXml->SetAttribute("name", "Z");
		paramXml->SetAttribute("type", "float"); 
	}

    void writeDefaultUvTechnique(const MeshPtr& mesh, TiXmlElement* sourceXml)
    {
        const size_t STRIDE = 2; // S, T
        size_t count = mesh->getVertices().size();

        TiXmlElement* techniqueCommonXml = new TiXmlElement("technique_common");
        sourceXml->LinkEndChild(techniqueCommonXml);

        TiXmlElement* accessorXml = new TiXmlElement("accessor");
        techniqueCommonXml->LinkEndChild(accessorXml);

        accessorXml->SetAttribute("count", boost::lexical_cast<std::string>(count));
        accessorXml->SetAttribute("stride", STRIDE);
        accessorXml->SetAttribute("source", '#' + createTextureCoordinatesArrayId(mesh));

        TiXmlElement* paramXml = new TiXmlElement("param");
        accessorXml->LinkEndChild(paramXml);
        paramXml->SetAttribute("name", "S");
        paramXml->SetAttribute("type", "float");
        paramXml = new TiXmlElement("param");
        accessorXml->LinkEndChild(paramXml);
        paramXml->SetAttribute("name", "T");
        paramXml->SetAttribute("type", "float");
    }

    

    void writePositionSource(const MeshPtr& mesh, TiXmlElement* meshXml)
	{
		TiXmlElement* sourceXml = new TiXmlElement("source");
		meshXml->LinkEndChild(sourceXml);

		sourceXml->SetAttribute("id", createPositionsId(mesh));

        std::vector<double> positions = join(mesh->getVertices(), joinPosition);
        
		writeFloatArray(createPositionsArrayId(mesh), positions, sourceXml);
		writeDefaultPositionTechnique(mesh, sourceXml);
	}

    void writeNormalsSource(const MeshPtr& mesh, TiXmlElement* meshXml)
    {
        TiXmlElement* sourceXml = new TiXmlElement("source");
        meshXml->LinkEndChild(sourceXml);

        sourceXml->SetAttribute("id", createNormalsId(mesh));

        std::vector<double> normals = join(mesh->getVertices(), joinNormal);

        writeFloatArray(createNormalsArrayId(mesh), normals, sourceXml);
        // TODO: Wrong! PositionsTechnique uses positions array
        writeDefaultPositionTechnique(mesh, sourceXml);
    }

    void writeTextureCoordinatesSource(const MeshPtr& mesh, TiXmlElement* meshXml)
    {
        TiXmlElement* sourceXml = new TiXmlElement("source");
        meshXml->LinkEndChild(sourceXml);

        sourceXml->SetAttribute("id", createTextureCoordinatesId(mesh));

        std::vector<double> textureCoordinates = join(mesh->getVertices(), joinTextureCoordinate);

        writeFloatArray(createTextureCoordinatesArrayId(mesh), textureCoordinates, sourceXml);
        writeDefaultUvTechnique(mesh, sourceXml);
    }

	void writeVertices(const MeshPtr& mesh, TiXmlElement* meshXml)
	{
		TiXmlElement* verticesXml = new TiXmlElement("vertices");
		meshXml->LinkEndChild(verticesXml);

		verticesXml->SetAttribute("id", createVerticesId(mesh));

		// Positions
        TiXmlElement* inputXml = new TiXmlElement("input");
		verticesXml->LinkEndChild(inputXml);

        inputXml->SetAttribute("source", '#' + createPositionsId(mesh));
		inputXml->SetAttribute("semantic", "POSITION");

        // Normals
        inputXml = new TiXmlElement("input");
        verticesXml->LinkEndChild(inputXml);

        inputXml->SetAttribute("source", '#' + createNormalsId(mesh));
        inputXml->SetAttribute("semantic", "NORMAL");

        // Texture coordinates
        inputXml = new TiXmlElement("input");
        verticesXml->LinkEndChild(inputXml);

        inputXml->SetAttribute("source", '#' + createTextureCoordinatesId(mesh));
        inputXml->SetAttribute("semantic", "TEXCOORD");
	}

	void writeMesh(const MeshPtr& mesh, TiXmlElement* geometryXml) 
	{
		TiXmlElement* meshXml = new TiXmlElement("mesh");
		geometryXml->LinkEndChild(meshXml);

		writePositionSource(mesh, meshXml);
        writeNormalsSource(mesh, meshXml);
        writeTextureCoordinatesSource(mesh, meshXml);
		writeVertices(mesh, meshXml);
		
        const Primitives& primitives = mesh->getPrimitives();

        // TODO: Enable/disable merges
        Primitives triangless;
        Primitives polygonss;


        for (Primitives::const_iterator it = primitives.begin(); it != primitives.end(); ++it)
        {
            PrimitivePtr primitive = *it;
            
            switch (primitive->getType())
            {
            case col::POLYGONS:
                {
                    PolygonsPtr polygons = boost::static_pointer_cast<Polygons>(primitive);
                    //writePolygons(polygons, mesh, meshXml);
                    polygonss.push_back(polygons);

                    break;
                }
            case col::TRIANGLES:
                {
                    TrianglesPtr triangles = boost::static_pointer_cast<Triangles>(primitive);
                    //writeTriangles(triangles, mesh, meshXml);
                    triangless.push_back(triangles);

                    break;
                }
            }
        }

        if (!triangless.empty())
        {
            writePolygons(triangless, mesh, meshXml);
        }

        if (!polygonss.empty())
        {
            writePolygons(polygonss, mesh, meshXml);
        }
	}

	void writeGeometry(const MeshPtr& mesh, TiXmlElement* libraryGeometriesXml) 
	{
		TiXmlElement* geometryXml = new TiXmlElement("geometry");
		libraryGeometriesXml->LinkEndChild(geometryXml);

		geometryXml->SetAttribute("id", createGeometryId(mesh));

		writeMesh(mesh, geometryXml);
	}

	void writeLibraryGeometries(const MeshLibrary& meshLibrary, TiXmlElement* colladaXml)
	{
        const Meshs& meshs = meshLibrary.getMeshs();

        if (!meshs.empty())
        {
		    TiXmlElement* libraryGeometriesXml = new TiXmlElement("library_geometries");
		    colladaXml->LinkEndChild(libraryGeometriesXml);

		    for (Meshs::const_iterator it = meshs.begin(); it != meshs.end(); ++it)
		    {   
			    writeGeometry(*it, libraryGeometriesXml);
		    }
        }
	}

	void writeInstanceGeometry(const MeshPtr& mesh, const MaterialPtr& material, TiXmlElement* nodeXml)
	{
		TiXmlElement* instanceGeometryXml = new TiXmlElement("instance_geometry");
		nodeXml->LinkEndChild(instanceGeometryXml);

		std::string idRef = '#' + createGeometryId(mesh);

		instanceGeometryXml->SetAttribute("url", idRef);

        if (mesh->hasMaterials())
        {
            TiXmlElement* bindMaterialXml = new TiXmlElement("bind_material");
            instanceGeometryXml->LinkEndChild(bindMaterialXml);

            TiXmlElement* techniqueCommonXml = new TiXmlElement("technique_common");
            bindMaterialXml->LinkEndChild(techniqueCommonXml);

            const Primitives& primitives = mesh->getPrimitives();

            boost::unordered_set<MaterialPtr> boundMaterials;

            for (Primitives::const_iterator it = primitives.begin(); it != primitives.end(); ++it)
            {
                const PrimitivePtr& primitive = *it;

                // A material for this primitive exists (use_count != 0)
                if (!primitive->getMaterial().expired())
                {
                    MaterialPtr material = primitive->getMaterial().lock();

                    boost::unordered_set<MaterialPtr>::const_iterator it = boundMaterials.find(material);

                    if (it == boundMaterials.end())
                    {
                        boundMaterials.insert(material);

                        std::string symbol = createMaterialReference(primitive);
                        // TODO: There is another higher scope material -> confusing

                        TiXmlElement* instanceMaterialXml = new TiXmlElement("instance_material");
                        techniqueCommonXml->LinkEndChild(instanceMaterialXml);
                        instanceMaterialXml->SetAttribute("symbol", symbol);
                        instanceMaterialXml->SetAttribute("target", '#' + material->getId());

                        if (material->getTexture())
                        {
                            TiXmlElement* bindVertexInputXml = new TiXmlElement("bind_vertex_input");
                            instanceMaterialXml->LinkEndChild(bindVertexInputXml);
                            bindVertexInputXml->SetAttribute("semantic", "TEX0");
                            bindVertexInputXml->SetAttribute("input_semantic", "TEXCOORD");
                            bindVertexInputXml->SetAttribute("input_set", "0");
                        }
                    }
                }                
            }

            
        }
	}

    void writeTitleAsset(const std::string& title, TiXmlElement* parentXml)
    {
        TiXmlElement* assetXml = new TiXmlElement("asset");
        parentXml->LinkEndChild(assetXml);
   
        TiXmlElement* titleXml = new TiXmlElement("title");
        assetXml->LinkEndChild(titleXml);

        TiXmlText* textXml = new TiXmlText(title);
        titleXml->LinkEndChild(textXml);
    }

    void writeNodeInstanceXml(const NodePtr& node, TiXmlElement* parentXml)
    {
        TiXmlElement* instanceNodeXml = new TiXmlElement("instance_node");
        parentXml->LinkEndChild(instanceNodeXml);

        instanceNodeXml->SetAttribute("url", '#' + node->getId());
    }

    void writeNodeXml(const NodePtr& node, TiXmlElement* parentXml)
    {
        TiXmlElement* nodeXml = new TiXmlElement("node");
        parentXml->LinkEndChild(nodeXml);

        nodeXml->SetAttribute("id", node->getId());

        nodeXml->SetAttribute("name", makeValidNCName(node->getName()));
        //writeTitleAsset(node->getName(), nodeXml);
        

        if (node->getTransformation())
        {
            TiXmlElement* matrixXml = new TiXmlElement("matrix");
            nodeXml->LinkEndChild(matrixXml);

            std::stringstream text;

            col::Matrix4 transformation = *node->getTransformation();
            text << transformation;
            
            TiXmlText* matrixText = new TiXmlText(text.str());
            matrixXml->LinkEndChild(matrixText);
        }

        if (node->getMesh().lock().get() != NULL)
        {
            writeInstanceGeometry(node->getMesh().lock(), node->getMaterial().lock(), nodeXml);
        }
        
        const Children& children = node->getChildren();
        for (Children::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            NodePtr node = it->get<0>();

            if (it->get<1>() == REFERENCE)
            {
                writeNodeInstanceXml(node, nodeXml);
            }
            else if (it->get<1>() == INSTANCE)
            {
                writeNodeXml(node, nodeXml);
            }
        }
    }

    void writeLibraryNodes(const Scene& scene, TiXmlElement* colladaXml)
    {

    }

	void writeLibraryVisualScenes(const Scene& scene, const NodeLibrary& nodesLibrary, TiXmlElement* colladaXml)
	{
        if (!scene.isEmpty())
        {
            // TODO:
            if (true)
            {
                // Visual scene part
                TiXmlElement* libraryVisualScenesXml = new TiXmlElement("library_visual_scenes");
                colladaXml->LinkEndChild(libraryVisualScenesXml);

                TiXmlElement* visualSceneXml = new TiXmlElement("visual_scene");
                libraryVisualScenesXml->LinkEndChild(visualSceneXml);		

                visualSceneXml->SetAttribute("id", "default-scene");

                {
                    const Nodes& children = scene.getChildren();
                    for (Nodes::const_iterator it = children.begin(); it != children.end(); ++it)
                    {
                        writeNodeXml(*it, visualSceneXml);
                    }
                }

                // Library nodes part

                {
                    const Nodes& children = nodesLibrary.getItems();

                    if (!children.empty())
                    {
                        TiXmlElement* libraryNodesXml = new TiXmlElement("library_nodes");
                        colladaXml->LinkEndChild(libraryNodesXml);

                        for (Nodes::const_iterator it = children.begin(); it != children.end(); ++it)
                        {
                            NodePtr node = *it;

                            // Nodes library output
                            writeNodeXml(*it, libraryNodesXml);
                        }
                    }
                }
            }
            else
            {
                TiXmlElement* libraryVisualScenesXml = new TiXmlElement("library_visual_scenes");
                colladaXml->LinkEndChild(libraryVisualScenesXml);

                // 		<visual_scene id="scene">
                // 			<node id="object">
                // 			<instance_geometry url="#test" />
                // 			</node>
                // 		</visual_scene>

                TiXmlElement* visualSceneXml = new TiXmlElement("visual_scene");
                libraryVisualScenesXml->LinkEndChild(visualSceneXml);		

                visualSceneXml->SetAttribute("id", "default-scene");

                const Nodes& children = scene.getChildren();

                for (Nodes::const_iterator it = children.begin(); it != children.end(); ++it)
                {
                    writeNodeXml(*it, visualSceneXml);
                }
            }
        }		
	}

	void writeScene(TiXmlElement* colladaXml)
	{
		TiXmlElement* sceneXml = new TiXmlElement("scene");
		colladaXml->LinkEndChild(sceneXml);

		// <instance_visual_scene url="#scene" />

		TiXmlElement* instanceVisualSceneXml = new TiXmlElement("instance_visual_scene");
		sceneXml->LinkEndChild(instanceVisualSceneXml);

		instanceVisualSceneXml->SetAttribute("url", "#default-scene");
	}

    void writeMaterial(const MaterialPtr& material, TiXmlElement* libraryMaterialsXml)
    {
        TiXmlElement* materialXml = new TiXmlElement("material");
        libraryMaterialsXml->LinkEndChild(materialXml);
        materialXml->SetAttribute("id", material->getId());

        TiXmlElement* instanceEffectXml = new TiXmlElement("instance_effect");
        materialXml->LinkEndChild(instanceEffectXml);
        instanceEffectXml->SetAttribute("url", "#effect-" + material->getId());
    }

    void writeLibraryMaterials(const MaterialLibrary& materialLib, TiXmlElement* colladaXml)
    {
        const Materials& materials = materialLib.getMaterials();

        if (materials.size() > 0)
        {
            TiXmlElement* libraryMaterialsXml = new TiXmlElement("library_materials");
            colladaXml->LinkEndChild(libraryMaterialsXml);

            for (Materials::const_iterator it = materials.begin(); it != materials.end(); ++it)
            {
                writeMaterial(*it, libraryMaterialsXml);
            }
        }  
    }

    void writeSurfaceParam(const TexturePtr& texture, TiXmlElement* profileCommonXml)
    {
        TiXmlElement* newParamXml = new TiXmlElement("newparam");
        profileCommonXml->LinkEndChild(newParamXml);
        newParamXml->SetAttribute("sid", createTextureSurfaceId(texture));

        // TODO:
        TiXmlElement* surfaceXml = new TiXmlElement("surface");
        newParamXml->LinkEndChild(surfaceXml);
        surfaceXml->SetAttribute("type", "2D");

        TiXmlElement* initFromXml = new TiXmlElement("init_from");
        surfaceXml->LinkEndChild(initFromXml);

        // TODO:
        TiXmlText* xmlText = new TiXmlText(texture->getImage()->getId());
        initFromXml->LinkEndChild(xmlText);
    }

    void writeSamplerParam(const TexturePtr& texture, TiXmlElement* profileCommonXml)
    {
        TiXmlElement* newParamXml = new TiXmlElement("newparam");
        profileCommonXml->LinkEndChild(newParamXml);
        newParamXml->SetAttribute("sid", createTextureSamplerId(texture));

        // TODO:
        TiXmlElement* sampler2DXml = new TiXmlElement("sampler2D");
        newParamXml->LinkEndChild(sampler2DXml);

        TiXmlElement* sourceXml = new TiXmlElement("source");
        sampler2DXml->LinkEndChild(sourceXml);

        TiXmlText* xmlText = new TiXmlText(createTextureSurfaceId(texture));
        sourceXml->LinkEndChild(xmlText);

        // WRAP is default
        if (texture->getWrap() != col::WRAP)
        {
            std::string wrapText = boost::lexical_cast<std::string>(texture->getWrap());

            TiXmlElement* wrapSXml = new TiXmlElement("wrap_s");
            sampler2DXml->LinkEndChild(wrapSXml);

            xmlText = new TiXmlText(wrapText);
            wrapSXml->LinkEndChild(xmlText);

            TiXmlElement* wrapTXml = new TiXmlElement("wrap_t");
            sampler2DXml->LinkEndChild(wrapTXml);

            xmlText = new TiXmlText(wrapText);
            wrapTXml->LinkEndChild(xmlText);
        }
    }

    void writeEffect(const MaterialPtr& material, TiXmlElement* effectsLibraryXml)
    {
        //      <effect id="red-effect">
        // 			<profile_COMMON>
        // 				<technique sid="COMMON">
        // 					<phong>
        // 						<diffuse>
        // 							<color>1 0 0 1</color>
        // 						</diffuse>
        // 					</phong>
        // 				</technique>
        // 			</profile_COMMON>
        // 		</effect>

        TiXmlElement* effectXml = new TiXmlElement("effect");
        effectsLibraryXml->LinkEndChild(effectXml);
        effectXml->SetAttribute("id", "effect-" + material->getId());

        TiXmlElement* profileCommonXml = new TiXmlElement("profile_COMMON");
        effectXml->LinkEndChild(profileCommonXml);

        // begin params
        if (material->getTexture())
        {
            writeSurfaceParam(*material->getTexture(), profileCommonXml);
            writeSamplerParam(*material->getTexture(), profileCommonXml);
        }
        // end params

        TiXmlElement* techniqueXml = new TiXmlElement("technique");
        profileCommonXml->LinkEndChild(techniqueXml);
        techniqueXml->SetAttribute("sid", "COMMON");

        TiXmlElement* phongXml = new TiXmlElement("lambert");
        techniqueXml->LinkEndChild(phongXml);

        TiXmlElement* diffuseXml = new TiXmlElement("diffuse");
        phongXml->LinkEndChild(diffuseXml);

        if (material->getTexture())
        {
            TiXmlElement* textureXml = new TiXmlElement("texture");
            diffuseXml->LinkEndChild(textureXml);

            std::string samplerId = createTextureSamplerId(*material->getTexture());

            textureXml->SetAttribute("texture", samplerId);
            textureXml->SetAttribute("texcoord", "TEX0");
        }
        else if (material->getColor())
        {
            Color color = *material->getColor();

            TiXmlElement* colorXml = new TiXmlElement("color");
            diffuseXml->LinkEndChild(colorXml);

            std::string colorText = boost::lexical_cast<std::string>(color);

            TiXmlText* colorTextXml = new TiXmlText(colorText);
            colorXml->LinkEndChild(colorTextXml);
        }
        else
        {
            // TODO: FAIL!!!
            // Empty materials result in invalid dom tree.
            // A material needs one of color or texture.
            //assert(false);
            throw std::runtime_error("A material needs either a texture or a color");
        }
        
    }

    void writeLibraryEffects(const MaterialLibrary& materialLib, TiXmlElement* colladaXml)
    {
        const Materials& materials = materialLib.getMaterials();

        if (!materials.empty())
        {
            TiXmlElement* libraryEffectsXml = new TiXmlElement("library_effects");
            colladaXml->LinkEndChild(libraryEffectsXml);

            for (Materials::const_iterator it = materials.begin(); it != materials.end(); ++it)
            {
                writeEffect(*it, libraryEffectsXml);
            }
        }               
    }

    void writeImage(const ImagePtr& image, TiXmlElement* libraryImagesXml)
    {
        TiXmlElement* imageXml = new TiXmlElement("image");
        libraryImagesXml->LinkEndChild(imageXml);

        imageXml->SetAttribute("id", image->getId());

        TiXmlElement* initFromXml = new TiXmlElement("init_from");
        imageXml->LinkEndChild(initFromXml);

        TiXmlText* xmlText = new TiXmlText(image->getPath().string());
        initFromXml->LinkEndChild(xmlText);
    }

    void writeLibraryImages(const ImageLibrary& imageLib, TiXmlElement* colladaXml)
    {
        const ImageLibrary::Items images = imageLib.getItems();

        if (!images.empty())
        {
            TiXmlElement* libraryImagesXml = new TiXmlElement("library_images");
            colladaXml->LinkEndChild(libraryImagesXml);

            for (ImageLibrary::Items::const_iterator it = images.begin(); it != images.end(); ++it)
            {
                writeImage(*it, libraryImagesXml);
            }
        }
    }

	Writer::Writer(const Context& context)
		: m_context(context)
        , m_scene(m_context.getScene())
        , m_meshLib(m_context.getMeshLibrary())
        , m_materialLib(m_context.getMaterialLibrary())
        , m_imageLib(m_context.getImageLibrary())
        , m_nodeLib(m_context.getNodeLibrary())
        , m_schemaLocationEnabled(false)
	{

	}

	void Writer::write(std::ostream& out)
	{
		TiXmlDocument doc;
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
		doc.LinkEndChild(decl);

		TiXmlElement* colladaXml = new TiXmlElement("COLLADA");
		doc.LinkEndChild(colladaXml);

		colladaXml->SetAttribute("xmlns", XMLNS);
		colladaXml->SetAttribute("version", VERSION);
        
        if (m_schemaLocationEnabled)
        {
            colladaXml->SetAttribute("schemaLocation", XMLNS + " " + SCHEMA_LOCATION);
        }

		writeAsset(colladaXml);
        writeLibraryImages(m_imageLib, colladaXml);
        writeLibraryEffects(m_materialLib, colladaXml);
        writeLibraryMaterials(m_materialLib, colladaXml);
		writeLibraryGeometries(m_meshLib, colladaXml);
		writeLibraryVisualScenes(m_scene, m_nodeLib, colladaXml);
		writeScene(colladaXml);	

		out << doc;
	}

    void write(Writer& writer, const std::string& path)
    {
        boost::iostreams::stream<boost::iostreams::file_sink> out(path);
        writer.write(out);
    }

    void write(Writer& writer, const std::wstring& path)
    {
        boost::filesystem::wpath p(path);
        boost::iostreams::stream<boost::iostreams::file_descriptor_sink> out(p);
        writer.write(out);
    }

}