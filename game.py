import pygame

from game_items import *



class Game():

    def __init__(self):
        self.main_window = pygame.display.set_mode((640, 480))
        pygame.display.set_caption('Snake')

        self.score_label=Lable()
        self.tip_label=Lable(24,False)
        self.is_game_over=False
        self.is_pause=False
        self.food=Food()
        self.snake=Snake()
        print(self.snake.body_list)

    def reset_game(self):
        self.is_game_over=False
        self.is_pause=False

        self.food.random_rect()
        self.snake.reset_snake()


    def start(self):
        clock=pygame.time.Clock()


        while True:
            for event in pygame.event.get():
                if event.type==pygame.QUIT:
                    return
                elif event.type==pygame.KEYDOWN:
                    if event.key==pygame.K_ESCAPE:
                        return
                    elif event.key == pygame.K_SPACE:
                        if self.is_game_over:
                            self.reset_game()
                        else:
                            self.is_pause=not self.is_pause
                if not self.is_pause and not self.is_game_over:
                    if event.type==FOOD_UPDATE_EVENT:
                        self.food.random_rect()
                    if event.type==SNAKE_UPDATE_EVENT:
                        self.is_game_over=not self.snake.update()
                    if event.type==pygame.KEYDOWN:
                        if event.key in (pygame.K_LEFT,pygame.K_RIGHT,
                                         pygame.K_UP,pygame.K_DOWN):
                            self.snake.change_dir(event.key)




            self.main_window.fill(BACKGROUND_COLOR)
            if self.is_game_over:
                self.tip_label.draw(self.main_window,'The game over,please press the space to start new game')
            elif self.is_pause:
                self.tip_label.draw(self.main_window,'The game pauses,please press space to continue')


            else:
                if self.snake.has_eat(self.food):
                    self.food.random_rect()
            self.score_label.draw(self.main_window,' score : %d'%self.snake.score)
            self.food.draw(self.main_window)
            self.snake.draw(self.main_window)

            pygame.display.update()


            clock.tick(60)


class Lable():
    def __init__(self,size=28,is_score=True):
        self.font=pygame.font.SysFont('mvboli',size)
        self.is_score=is_score
    def draw(self,window,text):
        color=SCORE_TEXT_COLOR if self.is_score else TIP_TEXT_COLOR
        text_surface=self.font.render(text,True,color)
        text_rect=text_surface.get_rect()

        window_rect=window.get_rect()

        if self.is_score:
            text_rect.bottomleft=window_rect.bottomleft
        else:
            text_rect.center=window_rect.center



        window.blit(text_surface,text_rect)






if __name__ == '__main__':
    pygame.init()
    Game()
    Game().start()
    pygame.quit()
