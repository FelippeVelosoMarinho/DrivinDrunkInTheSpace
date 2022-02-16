# DrivinDrunkInTheSpace
Trabalho final realizado em Allegro como parte das dependências da disciplina de Programação de Softwares da UFMG

## 1 - Introdução


Este documento tem como  objetivo servir como documentação para o trabalho prático final da disciplina de PDS I, onde deveríamos desenvolver um jogo eletrônico de gráfico semelhante às versões clássicas (antigas) da franquia R-type. RType é um videogame de fliperama de rolagem horizontal desenvolvido e lançado pela Irem em 1987.. Basicamente, o jogador deve se movimentar entre as fases desviando dos inimigos e cumprindo pequenos objetivos para seguir em frente tais quais: apertar botões, matar inimigos e etc. O jogo deveria ser desenvolvido em C utilizando a biblioteca Allegro e os conhecimentos adquiridos ao longo do curso.

## 2 - História do Jogo
	
Neste jogo você é um viajante espacial que está no ano de 2622 e após a derrota de seu time de futebol espacial do coração, acabou se embriagando e pegando a estrada… Mal sabia você que estava na contramão!!!
	O Boss irá aparecer após um minuto. “Aquilo é realmente um polvo mesmo ou você bebeu um pouco demais??? Será que os blocos são ônibus??? Os meteoros são carros??  Os jetskis são motos?? Os banhistas são pedestres???... Eéé enfim, mistérios… “


*Título do Jogo

## 3 - Instruções

### 3.1 - Descrição

Em Driving Drunk in the Space seu objetivo é sobreviver e derrotar o chefe. Logo após a iniciação do jogo, abrirá uma tela de menu, onde serão apresentadas as opções de jogar e sair. Ao apertar “Sair” o jogo finaliza, ao apertar “Jogar”, o jogo abre uma tela de tutorial onde haverá as informações básicas para o jogo. Ao executar o jogador possui somente uma vida, isto é, “encostou, morreu”,. Todos os meteoros podem ser destruídos com o tiro básico que destrói um inimigo mas se destrói no processo, e o tiro avançado que ao segurar a barra de espaço por um segundo, é liberado um poderoso tiro de raio maior que só se destrói ao passar dos limites da tela. A cada meteoro destruído, o tamanho dele será adicionado a sua pontuação, ou seja, meteoros maiores, maiores pontuações. A pontuação é exibida na tela assim como o tempo que ao decorrer 1 minuto é liberado o chefe, aparecendo também seus pontos de vida. O chefe possui 15 pontos de vida, onde o tiro básico tira 1 pontos e o tiro avançado 4. Mesmo com o chefe em tela, os inimigos continuam aparecendo.
	Ao derrotar o chefe é acrescentado 5000 pontos no score e exibido sua pontuação final por 3 segundos, igualmente se o jogador for morto. Se for record, é exibido um texto com “Novo Recorde!”.


*Tela de Menu


*Tela de Tutorial

### 3.2 - Telas do Jogo


*Tela do Jogo em ação


*Tela do Jogo - Batalha com o Chefe

*Tela do Jogo - Exibição da Pontuação
3.3 - Controles
	
	No menu e tutorial:
W, A, S, D ou  SETA CIMA, SETA ESQUERDA, SETA DIREITA, SETA BAIXO - Seleciona as caixas de seleção do menu.
		P - Inicia o jogo.

	Em jogo:
		W, A, S, D - Movimenta a Nave.
Clicando:
Espaço -  Atira - Tiro básico.

Pressionando: 
Espaço (1s) -  Atira - Tiro especial.


