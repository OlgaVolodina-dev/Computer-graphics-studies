#include "obj_reader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Log.h"
struct VertRef
{
    VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
    int v, vt, vn;
};

void ObjReader::ReadObj(std::string& filename, Model& model, BoundingBox& bb)
{

    std::ifstream in(filename.data(), std::ifstream::in);

    std::vector< glm::vec4 > positions(1, glm::vec4(0, 0, 0, 0));
    std::vector< glm::vec3 > texcoords(1, glm::vec3(0, 0, 0));
    std::vector< glm::vec3 > normals(1, glm::vec3(0, 0, 0));
    std::string lineStr;
    while (std::getline(in, lineStr))
    {
        std::istringstream lineSS(lineStr);
        std::string lineType;
        lineSS >> lineType;

        // vertex
        if (lineType == "v")
        {
            float x = 0, y = 0, z = 0, w = 1;
            lineSS >> x >> y >> z >> w;
            positions.push_back(glm::vec4(x, y, z, w));
            bb.maxX = std::max(x, bb.maxX);
            bb.maxY = std::max(y, bb.maxY);
            bb.maxZ = std::max(z, bb.maxZ);
            bb.minX = std::min(x, bb.minX);
            bb.minY = std::min(y, bb.minY);
            bb.minZ = std::min(z, bb.minZ);
        }

        // texture
        if (lineType == "vt")
        {
            float u = 0, v = 0, w = 0;
            lineSS >> u >> v >> w;
            v = 1.0 - v;
            texcoords.push_back(glm::vec3(u, v, w));
        }

        // normal
        if (lineType == "vn")
        {
            float i = 0, j = 0, k = 0;
            lineSS >> i >> j >> k;
            normals.push_back(glm::normalize(glm::vec3(i, j, k)));
        }
        if (lineType == "usemtl") {
            MeshInfo meshInfo;
            lineSS >> meshInfo.name;
            meshInfo.index = model.vertices.size();
            model.meshInfo.push_back(meshInfo);
        }
        // polygon
        if (lineType == "f")
        {
            std::vector< VertRef > refs;
            std::string refStr;
            while (lineSS >> refStr)
            {
                std::istringstream ref(refStr);
                std::string vStr, vtStr, vnStr;
                std::getline(ref, vStr, '/');
                std::getline(ref, vtStr, '/');
                std::getline(ref, vnStr, '/');
                int v = atoi(vStr.c_str());
                int vt = atoi(vtStr.c_str());
                int vn = atoi(vnStr.c_str());
                v = (v >= 0 ? v : positions.size() + v);
                vt = (vt >= 0 ? vt : texcoords.size() + vt);
                vn = (vn >= 0 ? vn : normals.size() + vn);
                refs.push_back(VertRef(v, vt, vn));
            }

            for (size_t i = 1; i + 1 < refs.size(); ++i)
            {
                const VertRef* p[3] = { &refs[0], &refs[i], &refs[i + 1] };

                glm::vec3 U(positions[p[1]->v] - positions[p[0]->v]);
                glm::vec3 V(positions[p[2]->v] - positions[p[0]->v]);
                glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));

                for (size_t j = 0; j < 3; ++j)
                {
                    Vertex vert;
                    vert.position = glm::vec3(positions[p[j]->v]);
                    vert.texCoord = glm::vec2(texcoords[p[j]->vt]);
                    vert.normal = (p[j]->vn != 0 ? normals[p[j]->vn] : faceNormal);
                    model.vertices.push_back(vert);
                }
            }
        }
    }
}

void ObjReader::ReadMTL(std::string& filename, Model& model)
{
    std::ifstream in(filename.data(), std::ifstream::in);

    std::string lineStr;
    MtlData * curMtlData = nullptr;
    while (std::getline(in, lineStr))
    
    {
        std::istringstream lineSS(lineStr);
        std::string lineType;
        lineSS >> lineType;

        auto fillVec3 = [&](glm::vec3 &tofill) {
            float x, y, z;
            lineSS >> x >> y >> z;
            tofill = glm::vec3(x, y, z);
        };

        if (lineType == "newmtl"){
            std::string name;
            lineSS >> name;
            for (auto &meshInfo : model.meshInfo) {
                if (meshInfo.name == name) {
                    curMtlData = &meshInfo.mtlData;
                }
            }
        }

        if (lineType == "Ns") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            lineSS >> curMtlData->Ns;
        }

        if (lineType == "Ka") {
            if (!curMtlData) { Log::Error("curMtlData is null");}

            fillVec3(curMtlData->Ka);
        }

        if (lineType == "Kd") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            fillVec3(curMtlData->Kd);
        }

        if (lineType == "Ks") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            fillVec3(curMtlData->Ks);
        }

        if (lineType == "Ni") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            lineSS >> curMtlData->Ni;
        }

        if (lineType == "d") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            lineSS >> curMtlData->d;
        }

        if (lineType == "illum") {
            if (!curMtlData) { Log::Error("curMtlData is null");}
            lineSS >> curMtlData->illum;
        }
    }
}