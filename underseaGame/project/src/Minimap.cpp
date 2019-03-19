#include "Minimap.h"

#include "EnemyManager.h"
#include "Utils.h"
#include "Game.h"

void Minimap::render(
	const Game& game) const {
  const Point2 BottomLeftCorner{game.screenSize.X() - offsetFromRight -
                                    minimapSize.X(),
                                offsetFromBottom};

  glScissor((int)BottomLeftCorner.X(), (int)BottomLeftCorner.Y(),
            (int)minimapSize.X(), (int)minimapSize.Y());
  glEnable(GL_SCISSOR_TEST);

  glPushMatrix();
  {

    glTranslatef(BottomLeftCorner.X(), BottomLeftCorner.Y(), 1.f);
    glScalef(minimapSize.X(), minimapSize.Y(), 1.f);

    // Draw minimap background
    {
      glBindTexture(GL_TEXTURE_2D, Texture::Sonar);
      glEnable(GL_TEXTURE_2D);

      glBegin(GL_QUADS);
      {
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.f, 0.f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.f, 1.f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.f, 1.f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.f, 0.f);
      }
      glEnd();

      glDisable(GL_TEXTURE_2D);
    }

    glPointSize(sonarPointSize);
    glPushMatrix();
    {

      glScalef(1.f / Map::width, 1.f / Map::width, 1.f);
      glTranslatef(Map::width / 2.f, Map::width / 2.f, 0.f);

      if (rotatesWithPlayer) {
        glRotatef(game.pat.getFacing() + 180.f, 0.f, 0.f, 1.f);
      }

      if (centersOnPlayer) {
        glTranslatef(-game.pat.getPosition().X(), -game.pat.getPosition().Z(), 0.f);

        
      }

      // Draw minimap dots
      glBegin(GL_POINTS);
      {
        // Draw player
        glColor3fv(playercolor);
        glVertex2f(game.pat.getPosition().X(), game.pat.getPosition().Z());

        // Draw enemies
        glColor3fv(enemyColor);
        for (const Point3 &enemyPosition : game.enemyManager.getEnemyPositions()) {
          glVertex2f(enemyPosition.X(), enemyPosition.Z());
        }

        // Draw jelly
        glColor3fv(jellyColor);
        for (const Point3 &jellyPosition : game.jellyManager.getJellyPositions(game)) {
          glVertex2f(jellyPosition.X(), jellyPosition.Z());
        }
      }
      glEnd();

  
    }
    glPopMatrix();
  }
  glPopMatrix();
  glColor3f(1.f,1.f,1.f);

  glDisable(GL_SCISSOR_TEST);

  utils::checkGLError(__FILE__, __LINE__);
}
