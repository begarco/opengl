/******************************************************************************\
*     Copyright (C) 2016 by Rémy Malgouyres                                    * 
*     http://malgouyres.org                                                    * 
*     File: light.hpp                                                          * 
*                                                                              * 
* The program is distributed under the terms of the GNU General Public License * 
*                                                                              * 
\******************************************************************************/ 

#ifndef HEADER_LIGHT_SOURCE_HPP
#define HEADER_LIGHT_SOURCE_HPP

#include <vector>
/**
 *  MÉTHODES CONCERNANT LES MODÈLES DE SOURCES LUMINEUSES
 */
struct LightSourceData{
    /** Classe représentant une source lumineuse */
  struct PointLightSource{
    int mLightId; // GL_LIGHT0, GL_LIGHT1, etc.
    float mLightPosition[4] = {0.0f,0.0f,10.0f,0.0f};  
    float mDiffuseIntensity[4] = {0.6f,0.6f,0.6f,1.0f}; // Intensité diffuse
    float mSpecularIntensity[4] = {0.6f,0.6f,0.6f,1.0f}; // Intensité spéculaire
  
    /** Constructeur initialisant toutes les propriétés de la source */
    PointLightSource(int lightId,
                float lightPositionX, float lightPositionY, float lightPositionZ, 
                float diffuseIntensityR, float diffuseIntensityG, float diffuseIntensityB, 
                float specularIntensityR, float specularIntensityG, float specularIntensityB)
      :mLightId(lightId)
    {
      mLightPosition[0] = lightPositionX;
      mLightPosition[1] = lightPositionY;
      mLightPosition[2] = lightPositionZ;
      mLightPosition[3] = 1.0;
      mDiffuseIntensity[0] = diffuseIntensityR;
      mDiffuseIntensity[1] = diffuseIntensityG;
      mDiffuseIntensity[2] = diffuseIntensityB;
      mDiffuseIntensity[3] = 1.0;
      mSpecularIntensity[0] = specularIntensityR;
      mSpecularIntensity[1] = specularIntensityG;
      mSpecularIntensity[2] = specularIntensityB;
      mSpecularIntensity[3] = 1.0;
    }
    
    /** Positionner la source dans le repère courant */
    void ApplyPosition() const{
      RenderingModel::ApplyPointLightPosition(mLightId, mLightPosition);
    }
    
    /** Applique l'intensité de la source et active la source */
    void ApplyIntensity() const{
      RenderingModel::ApplyPointLightIntensity(mLightId,
				    mDiffuseIntensity, mSpecularIntensity);
    }
    
    /** Désactive la source */
    void Disable() const {
        RenderingModel::DisablePointLight(mLightId);
    }
  };
  
  std::vector<PointLightSource> mSourcesRepereCamera; 
  std::vector<PointLightSource> mSourcesRepereMonde;
    
  /** Constructeur par défaut (ne crée aucune source lumineuse) */
  LightSourceData()
  :mSourcesRepereCamera(),
  mSourcesRepereMonde()
   {}
   
  /** Destructeur */
  ~LightSourceData() = default;
  
  /** Obtient l'ensemble des sources à placer dans un certain repère 
   * @param reverse true pour les sources de l'autre repère que typeRepere **/
  std::vector<PointLightSource>& GetSourcesByRepere(Camera::TypeRepere typeRepere,
                                        bool reverse = false){
    if (typeRepere == Camera::TypeRepere::CAMERA){
      return reverse ? mSourcesRepereMonde : mSourcesRepereCamera;
    }
    return reverse ? mSourcesRepereCamera : mSourcesRepereMonde;
  }
  
  /** Ajoute une source lumineuse dans un certain repère
   * @return true en case de succès, false en cas d'ID lightId déjà pris */
  bool AddSource(Camera::TypeRepere typeRepere, int lightId,
	  float lightPositionX, float lightPositionY, float lightPositionZ, 
	  float diffuseIntensityR, float diffuseIntensityG, float diffuseIntensityB, 
	  float specularIntensityR, float specularIntensityG, float specularIntensityB){
	    
    auto& sourcesAutre = GetSourcesByRepere(typeRepere, true);
    // Test de doublon pour l'ID de la source
    for (auto it = sourcesAutre.cbegin() ; it != sourcesAutre.cend() ; it++){
        if (it->mLightId == lightId){
          return false;
        }
    }
    auto& sources = GetSourcesByRepere(typeRepere);
     // Test de doublon pour l'ID de la source
    for (auto it = sources.cbegin() ; it != sources.cend() ; ++it){
        if (it->mLightId == lightId){
          return false;
        }
    }
  
    GetSourcesByRepere(typeRepere).push_back(PointLightSource(lightId,
                lightPositionX, lightPositionY, lightPositionZ, 
                diffuseIntensityR, diffuseIntensityG, diffuseIntensityB, 
                specularIntensityR, specularIntensityG, specularIntensityB));
    return true;
  }
  
  /** Supprime une source lumineuse dans un certain repère
   * La méthode est un peu lourde pour garder des tableaux contigus.
   * @return true en case de succès, false en case d'ID inexistant */
  bool DeleteSource(int lightId){
    auto& sourcesCamera = mSourcesRepereCamera;
    // Test de doublon pour l'ID de la source
    for (auto it = sourcesCamera.begin() ; it != sourcesCamera.end() ; ++it){
        if (it->mLightId == lightId){
	  sourcesCamera.erase(it); // suppression de l'élément
          return true;
        }
    }
    auto &sourcesMonde = mSourcesRepereMonde;
    // Test de doublon pour l'ID de la source
    for (auto it = sourcesMonde.begin() ; it != sourcesMonde.end() ; ++it){
        if (it->mLightId == lightId){
	  sourcesMonde.erase(it);
         return true;
        }
    }
   return false;
  }
  
  /** Positionner les sources qui se trouvent dans un certain repère */
  void ApplyLightPositions(Camera::TypeRepere typeRepere){
    auto sources = GetSourcesByRepere(typeRepere);
    for (auto it = sources.cbegin() ; it != sources.cend() ; ++it){
      it->ApplyPosition();
    }
  }
  
  /** Applique les intensités de toutes les sources de la scène */
  void ApplyLightIntensities(){
    auto& sourcesCamera = mSourcesRepereCamera;
    for (auto it = sourcesCamera.cbegin() ; it != sourcesCamera.cend() ; ++it){
      it->ApplyIntensity();
    }
    auto& sourcesMonde = mSourcesRepereMonde;
    for (auto it = sourcesMonde.cbegin() ; it != sourcesMonde.cend() ; ++it){
      it->ApplyIntensity();
    } 
  }
  
  /** Applique les intensités de toutes les sources de la scène */
  void DisbaleLightSources(Camera::TypeRepere typeRepere){
    auto &sources = GetSourcesByRepere(typeRepere);
    for (auto it = sources.cbegin() ; it != sources.cend() ; ++it){
      it->Disable();
    }
  }
};
#endif
