#ifndef DEATHSTARBEAM_H
#define DEATHSTARBEAM_H

#include <QTime>
#include <QPainter>
#include "animation.h"
#include "grid.h"

#define TARGET_SPEED 3

class DeathStarBeam : public Animation
{
public:
	/* Crée un rayon de l'étoile sur la Grille donnée et dont les cases ont la taille donnée.
	* Doit être créé suite à un signal dans le GameWidget (pour affichage).
	* Le rayon sera ajouté à la pile des animations du GameWidget.
	* L'entier player identifie le créateur du rayon et par conséquent, les cases qui lui appartiennent (ou non).
	*/
    DeathStarBeam(Grid* grid, QSize tileSize, Tile::TileType playerTile);
	
	/* Calcul le déplacement du rayon et renvoie l'image qui en découle.
	* Si la cible n'est pas atteinte, renvoie m_targetPixmap.
	* Sinon, calcul brièvement un rayon qui explose (évaluer de façon empirique la vitesse).
	* Le mieux restant de déplacer le viseur par "accoups" (avec une accélération et deccélération brusque).
	* Les "accoups" devraient de préférence chercher une zone plutôt proche afin de donner un aspect errant au viseur.
	* QPainter pour dessiner le rayon (QPainter painter(&pixmap) pour démarrer le painter sur une pixmap).
	*/
	virtual QPixmap nextFrame();
	
	/* Retourne vraie si l'animation est terminée.
	* C'est à dire quand la cible a été vérouillée et que l'animation de "l'explosion" est terminée.
	* On profite d'avoir accès à la grille pour modifier la valeur des cases sur un 3x3 autour du centre.
	*/
	virtual bool animationDone() const;
	
    QPoint target() const;
    void setTarget(const QPoint &target);

protected:
    // Renvoie vrai si le rayon a atteint sa cible.
    bool targetReached() const;
    bool targetValid() const;
    QPoint targetTile() const;
    void turnInto(const QPoint &point);
	
/*	Attributs hérités de Animation
protected:
    int m_frame;				Utile pour l'état de l'explosion.
    QList<QPixmap> m_frames;	Inutile ici (sauf pour pré-calculer l'explosion).
    QPoint m_position;			Position actuelle du viseur/de l'explosion.
*/

private:
	// Cible actuelle du viseur/de l'explosion.
    QPoint m_target;
    QSize m_tileSize;
	// Grille permettant de vérifier si oui ou non on est arrivé sur une cible valide.
    Grid *m_grid;
	// Image à charger dans le constructeur qui donnera le viseur.
	// A redimensionner : Hauteur = Largeur = min(Hauteur de case, Largeur de case) * 3 !
	QPixmap m_targetPixmap;
    qreal m_pixmapAngle;
	// Numéro du joueur qui a activé ce pouvoir.
    Tile::TileType m_playerTileType;
    Tile::TileType m_targetType;
    bool m_targetLocked;
};

#endif
