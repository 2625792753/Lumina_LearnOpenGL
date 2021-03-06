#pragma once

#include <vector>

#include "./DirLight.hpp"
#include "./PointLight.hpp"
#include "./SpotLight.hpp"

class LightManager{

public:
    std::vector<DirLight> dirlights;
    std::vector<PointLight> pointlights;
    std::vector<SpotLight> spotlights;

    int lights_num = 0;

    void CountLight() {
        this->lights_num = dirlights.size() + pointlights.size() + spotlights.size();
    }

    void ShaderConfig(Shader *shader) {
        Tools::ShaderCheck(shader);

        shader->Use();

        // LightInfo Block
        shader->setInt("lightinfo.num_dirlight", dirlights.size());
        shader->setInt("lightinfo.num_pointlight", pointlights.size());
        shader->setInt("lightinfo.num_spotlight", spotlights.size());

        for (int slot = 0; slot < dirlights.size(); ++slot)
            shader->setMat4("lightinfo.DirLight_Transform[" + std::to_string(slot) + "]", dirlights.at(slot).lightMatrix);

        // DirLights
        for (int slot = 0; slot < dirlights.size(); ++slot) {
            shader->setVec3("dirlights[" + std::to_string(slot) + "].direction", dirlights.at(slot).direction);
            shader->setVec3("dirlights[" + std::to_string(slot) + "].attrib.ambient", dirlights.at(slot).attrib.ambient);
            shader->setVec3("dirlights[" + std::to_string(slot) + "].attrib.diffuse", dirlights.at(slot).attrib.diffuse);
            shader->setVec3("dirlights[" + std::to_string(slot) + "].attrib.specular", dirlights.at(slot).attrib.specular);
        }

        // PointLights
        for (int slot = 0; slot < pointlights.size(); ++slot) {
            shader->setVec3("pointlights[" + std::to_string(slot) + "].position", pointlights.at(slot).position);
            shader->setFloat("pointlights[" + std::to_string(slot) + "].far", pointlights.at(slot).far);
            shader->setVec3("pointlights[" + std::to_string(slot) + "].attrib.ambient", pointlights.at(slot).attrib.ambient);
            shader->setVec3("pointlights[" + std::to_string(slot) + "].attrib.diffuse", pointlights.at(slot).attrib.diffuse);
            shader->setVec3("pointlights[" + std::to_string(slot) + "].attrib.specular", pointlights.at(slot).attrib.specular);
            shader->setFloat("pointlights[" + std::to_string(slot) + "].attenuation.constant", pointlights.at(slot).attenuation.constant);
            shader->setFloat("pointlights[" + std::to_string(slot) + "].attenuation.linear", pointlights.at(slot).attenuation.linear);
        }

        // SpotLight
        for (int slot = 0; slot < spotlights.size(); ++slot) {
            shader->setVec3("spotlights[" + std::to_string(slot) + "].direction", spotlights.at(slot).direction);
            shader->setVec3("spotlights[" + std::to_string(slot) + "].position", spotlights.at(slot).position);
            shader->setVec3("spotlights[" + std::to_string(slot) + "].attrib.ambient", spotlights.at(slot).attrib.ambient);
            shader->setVec3("spotlights[" + std::to_string(slot) + "].attrib.diffuse", spotlights.at(slot).attrib.diffuse);
            shader->setVec3("spotlights[" + std::to_string(slot) + "].attrib.specular", spotlights.at(slot).attrib.specular);
            shader->setFloat("spotlights[" + std::to_string(slot) + "].attenuation.constant", spotlights.at(slot).attenuation.constant);
            shader->setFloat("spotlights[" + std::to_string(slot) + "].attenuation.linear", spotlights.at(slot).attenuation.linear);

            shader->setFloat("spotlights[" + std::to_string(slot) + "].cutoff", glm::cos(glm::radians(spotlights.at(slot).cutoff)));
            shader->setFloat("spotlights[" + std::to_string(slot) + "].outer_cutoff", glm::cos(glm::radians(spotlights.at(slot).outtercutoff)));
        }

        // Shadow Maps Bindings
        // GL_TEXTURE7 ~ 16 used for ShadowMapping
        int slot = 0;
        for (int i = 0; i < dirlights.size(); ++i) {
            ++slot;
            shader->setInt("dirlights[" + std::to_string(i) + "].shadowmap", 7 + slot);
            glActiveTexture(GL_TEXTURE7 + slot);
            glBindTexture(GL_TEXTURE_2D, dirlights.at(i).depthmap);
        }
        
        for (int i = 0; i < pointlights.size(); ++i) {
            ++slot;
            shader->setInt("pointlights[" + std::to_string(i) + "].shadowmap", 7 + slot);
            glActiveTexture(GL_TEXTURE7 + slot);
            glBindTexture(GL_TEXTURE_CUBE_MAP, pointlights.at(i).depthmap);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};