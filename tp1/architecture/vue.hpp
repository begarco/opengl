/******************************************************************************\
*     Copyright (C) 2016 by Rémy Malgouyres                                    * 
*     http://malgouyres.org                                                    * 
*     File: vue.hpp                                                            * 
*                                                                              * 
* The program is distributed under the terms of the GNU General Public License * 
*                                                                              * 
\******************************************************************************/ 

#ifndef HEADER_VUE_HPP
#define HEADER_VUE_HPP

/**
 * CLASSE DE GESTION DE L'AFFICHAGE
 */
struct DisplayManager{
  
  GLint mLargeurFenetre; // Largeur de la vue
  GLint mHauteurFenetre; // Hauteur de la vue
  
  // Modèle de données à afficher
  Modele mModele;
  Camera mCamera;
  
   /** @brief  Constructeur prenant la géométrie de la fenêtre
   * Initialise les données nécessaires à l'affichage.*/
  DisplayManager(GLint largeurFenetre,GLint hauteurFenetre)
    :mLargeurFenetre(largeurFenetre),
    mHauteurFenetre(hauteurFenetre),
    mModele(), // Construction du modèle
    mCamera(0,0,0,1,0,0,0,1,0,10,100, 50)
  {
    FramesData::Init();
  }
  
  /** Méthode d'affichage */
  void Affichage(){
    // Affichage des Frames par seconde (FPS)
    if (FramesData::Update()){
      fprintf(stderr, "%s\n", FramesData::getDescriptionFPS());
    }
    // On efface le buffer vidéo (fenêtre graphique)
    glClearColor(mModele.mNiveauGris, mModele.mNiveauGris, 
                 mModele.mNiveauGris, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }

  /** Réglage du cadrage pour la vue
   * Doit être rappelée si la taille de la vue change (SDL_WINDOWEVENT)
   * @param l largeur de la (nouvelle) vue
   * @param h hauteur de la (nouvelle) vue
   */
  void Redimensionnement(GLint l,GLint h){
    mLargeurFenetre = l;
    mHauteurFenetre = h;
    // Surface de rendu (voir chapitres suivants)
    glViewport((GLint)mCamera.mPosition[0]-(l/2),(GLint)mCamera.mPosition[1]-(h/2),(GLsizei)l,(GLsizei)h);
    mCamera.Redimensionnement();
    
  }
};
#endif
