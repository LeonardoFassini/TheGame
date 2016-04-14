/* Criado por: Leonardo Tironi Fassini.
   E-mail: Leehtironi@gmail.com.
   Caso encontre algum bug, favor avisar.
   Todos os direitos reservados.
   By the way, as fontes e imagens podem ser baixadas nesse link:
   https://mega.nz/#F!xtY3iDSQ!KUmZz4pgqRZatMuEHiBHwghttps://mega.nz/#F!xtY3iDSQ!KUmZz4pgqRZatMuEHiBHwg
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
const int FRAMES_POR_SEGUNDO = 60;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

int main()
{
  ALLEGRO_BITMAP *imagem = NULL, *botao_jogar = NULL, *botao_sair = NULL;
  ALLEGRO_EVENT evento, ocorrencia;
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_AUDIO_STREAM *musica = NULL;
  ALLEGRO_SAMPLE *sample = NULL;
  int dentro_botao_jogar = 0, dentro_botao_sair = 0;
  int sair = 0;
  int game_over = 0, dir_x = 1, dir_y = 1, enter;
  double tempoInicial = 0;
  float raio = 5;
  float x = 320;
  float y = 240;
  int esquerdox = 1, esquerdoy = 200, direitox = 634, direitoy = 200;
  int altura;
  int largura;
  int frame = 0;
  
  /**************** Inicialização ****************/
  if (!al_init())
    {
      fprintf(stderr, "Falha ao inicializar a Allegro.\n");
      return -1;
    }
  
  /******************* Janela *******************/
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela)
    {
      fprintf(stderr, "Falha ao criar janela.\n");
      return -1;
    }
  
  /******************* Audio *******************/
  if(!al_install_audio())
    {
      fprintf(stderr, "Falha ao iniciar addon de audio 1");
      return -1;
    }
  
  if(!al_init_acodec_addon())
    {
      fprintf(stderr, "Falha ao iniciar o addon de audio 2");
      return -1;
    }
  
  if(!al_reserve_samples(1))
    {
      fprintf(stderr, "Falha ao alocar canais de audio");
      return -1;
    }
  
  musica = al_load_audio_stream("Didiana.ogg", 4, 1024);
  if (!musica)
    {
      fprintf(stderr, "Falha ao carregar audio.\n");
      al_destroy_event_queue(fila_eventos);
      al_destroy_display(janela);
      al_destroy_sample(sample);
      return false;
    }
  
  sample = al_load_sample("Bolinha.ogg");
  if(!sample)
    {
      fprintf(stderr, "Falha em criar som da bolinha\n");
      return -1;
    }

  /******************* Mouse *******************/
  if(!al_install_mouse())
    {
      fprintf(stderr, "Falha ao iniciar o mouse\n");
      al_destroy_display(janela);
      return -1;
    }
  if(!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
      fprintf(stderr, "Falha na atribuição do ponteiro do mouse\n");
      al_destroy_display(janela);
      return -1;
    }

  /****************** Teclado ******************/
  if(!al_install_keyboard())
    {
      fprintf(stderr, "Falha ao inicializar teclado.");
      return -1;
    }

  /******************* Fonte *******************/
  al_init_font_addon();
  if(!al_init_ttf_addon())
    {
      fprintf(stderr, "Falha ao iniciar addon allegro tff\n");
      return -1;
    }

  fonte = al_load_font("finalf.ttf", 24, 0);
  if(!fonte)
    {
      al_destroy_display(janela);
      fprintf(stderr, "Falha ao carregar fonte.\n");
      return -1;
    }

  /******************* Imagem *******************/
  if (!al_init_image_addon())
    {
      fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
      return -1;
    }
  imagem = al_load_bitmap("IMAGEM.jpg");
  if(!imagem)
    {
      fprintf(stderr, "Falha ao iniciar a imagem\n");
      al_destroy_display(janela);
      return -1;
    }

  /******************* Título *******************/
  al_set_window_title(janela, "Super Wooper Dooper");

  /**************** Primitivas *****************/
  if(!al_init_primitives_addon())
    {
      fprintf(stderr, "Falha na iniciação das primitivas.\n");
      return -1;
    }

/******************* Botões *******************/
  botao_jogar = al_create_bitmap(213, 40);
  if(!botao_jogar)
    {
      fprintf(stderr, "Falha ao criar o botao de jogar");
      al_destroy_bitmap(botao_jogar);
      al_destroy_display(janela);
      return -1;
    }

  botao_sair = al_create_bitmap(213, 40);
  if (!botao_sair)
    {
      fprintf(stderr, "Falha ao criar botão de saída.\n");
      al_destroy_bitmap(botao_sair);
      al_destroy_display(janela);
      return -1;
    }
  
  /******************* Eventos *******************/
  fila_eventos = al_create_event_queue();
  if (!fila_eventos)
    {
      fprintf(stderr, "Falha ao criar fila de eventos.\n");
      al_destroy_display(janela);
      return -1;
    }
  al_register_event_source(fila_eventos, al_get_mouse_event_source());
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_draw_bitmap(imagem, 0, 0, 0);
  al_flip_display();
  while(!sair)
    {
      game_over = 0;
      x = 320;
      y = 240;
      while(!al_is_event_queue_empty(fila_eventos))
	{
	  if (!al_is_event_queue_empty(fila_eventos))
	    {
	      al_wait_for_event(fila_eventos, &evento);
	      
	      if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
		  sair = true;
		}
	    }
	  if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
	    {
	      if(evento.mouse.x >= 213 &&
		 evento.mouse.x <= 426 &&
		 evento.mouse.y >= 120 &&
		 evento.mouse.y <= 160)
		{
		  dentro_botao_jogar = 1;
		}
	      else
		{
		  dentro_botao_jogar = 0;
		}
	      if(evento.mouse.x >= 213 &&
		 evento.mouse.x <= 426 &&
		 evento.mouse.y >= 360 &&
		 evento.mouse.y <= 400)
		{
		  dentro_botao_sair = 1;
		}
	      else
		{
		  dentro_botao_sair = 0;
		}
	    }
	  // verificar se eu soltei o botao
	  else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	    {
	      if(evento.mouse.x >= 213 &&
		 evento.mouse.x <= 426 &&
		 evento.mouse.y >= 360 &&
		 evento.mouse.y <= 400)
		{
		  sair = 1;
		}
	      if(evento.mouse.x >= 213 &&
		 evento.mouse.x <= 426 &&
		 evento.mouse.y >= 120 &&
		 evento.mouse.y <= 160)
		{
		  altura = 40;
		  largura = 5;
		  enter = 1;
		  esquerdox = 1;
		  esquerdoy = 200;
		  direitox = 634;
		  direitoy = 200;
		  while(game_over == 0)
		    { 
		      tempoInicial = al_get_time();
		      al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
		      al_set_audio_stream_playing(musica, true);
		      al_draw_bitmap(imagem, 0, 0, 0);
		      al_draw_filled_circle(x, y, raio, al_map_rgb(255, 0, 0));
		      al_draw_filled_rectangle(esquerdox, esquerdoy, esquerdox + largura, esquerdoy + altura, al_map_rgb(255, 255, 255));
		      al_draw_filled_rectangle(direitox, direitoy, direitox + largura, direitoy + altura, al_map_rgb(255, 255, 255));
		      al_flip_display();
		      while(enter == 1)
			{
			  al_draw_text(fonte, al_map_rgb(255, 255, 255), 250, 130,ALLEGRO_ALIGN_LEFT, "ENTER TO START");
			  al_flip_display();
			  al_wait_for_event(fila_eventos, &ocorrencia);
			  if(ocorrencia.keyboard.keycode == ALLEGRO_KEY_ENTER)
			    {
			      enter = 0;
			    }
			}
		      while(!al_is_event_queue_empty(fila_eventos))
			{
			  al_get_next_event(fila_eventos, &ocorrencia);
			  if(ocorrencia.keyboard.keycode)
			    {
			      switch(ocorrencia.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
				  {
				    if(direitoy <= 0)
				      {
					direitoy = 0;
				      }
				    direitoy -= 5;
				    break;
				  }
				case ALLEGRO_KEY_DOWN:
				  {
				    if(direitoy >= 440)
				      {
					direitoy = 440;
				      }
				    direitoy += 5;
				    break;
				  }
				case ALLEGRO_KEY_W:
				  {
				    if(esquerdoy <= 0)
				      {
					esquerdoy = 0;
				      }
				    esquerdoy -= 5;
				    break;
				  }
				case ALLEGRO_KEY_S:
				  {
				    if(esquerdoy >= 440)
				      {
					esquerdoy = 440;
				      }
				    esquerdoy += 5;
				    break;
				  }
				case ALLEGRO_KEY_ESCAPE:
				  {
				    game_over = 1;
				    break;
				  }
				}
			    }
			}
		      x += 2.0 * dir_x;
		      y += 2.0 * dir_y;
		      if(y >= esquerdoy && y <= esquerdoy + (altura/2) && esquerdox + largura >= x - raio)
			{
			  dir_x = 1;
			  dir_y = -1;
			  x= esquerdox + largura + raio;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(y > esquerdoy + (altura/2) && y <= esquerdoy + altura && esquerdox + largura >= x-raio)
			{
			  dir_x = 1;
			  dir_y = 1;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(y >= direitoy && y <= direitoy + (altura/2) && direitox <= x + raio)
			{
			  dir_x = -1;
			  dir_y = -1;
			  x = direitox - raio;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(y > direitoy + (altura/2) && y <= direitoy + altura && direitox <= x + raio)
			{
			  dir_x = -1;
			  dir_y = 1;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(y >= ALTURA_TELA - raio)
			{
			  dir_y = -1;
			  y = ALTURA_TELA - raio;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(y <= raio)
			{
			  dir_y = 1;
			  y = raio;
			  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		      if(x + raio >= LARGURA_TELA)
			{ 
			  al_draw_text(fonte, al_map_rgb(255, 255, 255), 300, 240, ALLEGRO_ALIGN_CENTRE, "PLAYER ONE WIN!");
			  al_flip_display();
			  al_rest(3.0);
			  game_over = 1;
			}		          
		      if(x - raio <= 0)
			{
			  al_draw_text(fonte, al_map_rgb(255, 255, 255), 300, 240, ALLEGRO_ALIGN_CENTRE, "PLAYER TWO WIN!");
			  al_flip_display();
			  al_rest(3.0);
			  game_over = 1;
			}
		      frame++;
		      if ((al_get_time() - tempoInicial) < 1.0 / FRAMES_POR_SEGUNDO)
			 {
			   al_rest((1.0 / FRAMES_POR_SEGUNDO) - (al_get_time() - tempoInicial));
			 }
		    }
		}
	    }
	}
      // desenho da imagem
      al_draw_bitmap(imagem, 0, 0, 0);
      // mudar bitmap p/ retangulo e colorir ele
      al_set_target_bitmap(botao_jogar);
      if(!dentro_botao_jogar)
	{
	  al_clear_to_color(al_map_rgb(0, 0, 255));
	}
      else
	{
	  al_clear_to_color(al_map_rgb(0, 191, 255));
	}
      al_set_target_bitmap(botao_sair);
      if(!dentro_botao_sair)
	{
	  al_clear_to_color(al_map_rgb(255, 0, 0));
	}
      else
	{
	  al_clear_to_color(al_map_rgb(255, 69, 0));
	}
      // mudar para janela e criar os retangulos
      al_set_target_bitmap(al_get_backbuffer(janela));
      al_draw_bitmap(botao_jogar, 213, 120, 0);
      al_draw_bitmap(botao_sair, 213, 360, 0);
      // desenhar os textos
      al_draw_text(fonte, al_map_rgb(255, 255, 255), 300, 130, ALLEGRO_ALIGN_CENTRE, "PLAY");
      al_draw_text(fonte, al_map_rgb(255, 255, 255), 300, 370, ALLEGRO_ALIGN_CENTRE, "EXIT");
      al_flip_display();
    }
  // finalizar o programa
  al_destroy_audio_stream(musica);
  al_destroy_sample(sample);
  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
  return 0;
}
// usar -lallegro_image -lallegro -lallegro_ttf -lallegro_font -lallegro -lallegro_primitives -lallegro_audio -lallegro_acodec
