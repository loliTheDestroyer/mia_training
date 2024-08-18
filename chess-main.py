import pygame

pygame.init()
board_dim = [800, 850]
screen = pygame.display.set_mode(board_dim)
pygame.display.set_caption('Chess')
font = pygame.font.Font('freesansbold.ttf', 20)

white_pieces = ['rook', 'knight', 'bishop', 'king', 'queen', 'bishop', 'knight', 'rook',
                'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn']
white_locations = [(0, 0), (1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0), (7, 0),
                   (0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1), (7, 1)]
black_pieces = ['rook', 'knight', 'bishop', 'king', 'queen', 'bishop', 'knight', 'rook',
                'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn', 'pawn']
black_locations = [(0, 7), (1, 7), (2, 7), (3, 7), (4, 7), (5, 7), (6, 7), (7, 7),
                   (0, 6), (1, 6), (2, 6), (3, 6), (4, 6), (5, 6), (6, 6), (7, 6)]

turn = 0
selection = 100
possible_moves = []
white_images = []
black_images = []
piece_list = ['pawn', 'queen', 'king', 'knight', 'rook', 'bishop']
for i in piece_list:
    image_path = f'game pieces/white {i}.png'
    white_images.append(pygame.image.load(image_path))
    image_path = f'game pieces/black {i}.png'
    black_images.append(pygame.image.load(image_path))
for i in range(len(white_images)):
    white_images[i] = pygame.transform.scale(white_images[i], (80, 80))
    black_images[i] = pygame.transform.scale(black_images[i], (80, 80))
counter = 0
run = True
def draw_board():
    screen.fill((128, 128, 128))
    for i in range(32):
        column = i % 4
        row = i // 4
        if row % 2 == 0:
            pygame.draw.rect(screen, (210, 210, 210), [600 - (column * 200), row * 100, 100, 100])
        else:
            pygame.draw.rect(screen, (210, 210, 210), [700 - (column * 200), row * 100, 100, 100])

        status_text = ['Turn: White', 'Turn: White: move the piece',
                       'Turn: Black', 'Turn: Black: move the piece']
        screen.blit(font.render(status_text[turn], True, 'black'), (20, 820))

        for i in range(len(white_pieces)):
            index = piece_list.index(white_pieces[i])
            screen.blit(white_images[index], (white_locations[i][0] * 100 + 10, white_locations[i][1] * 100 + 10))
        for i in range(len(black_pieces)):
            index = piece_list.index(black_pieces[i])
            screen.blit(black_images[index], (black_locations[i][0] * 100 + 10, black_locations[i][1] * 100 + 10))


def check_pawn(position, turn):
    moves_list = []
    x = position[0]
    y = position[1]

    if turn > 2:
        if (x, y + 1) not in white_locations and (x, y + 1) not in black_locations and y < 7:
            moves_list.append((x, y + 1))
        if (x, y + 2) not in white_locations and (x, y + 2) not in black_locations and y == 1:
            moves_list.append((x, y + 2))
        if (x + 1, y + 1) in black_locations:
            moves_list.append((x + 1, y + 1))
        if (x - 1, y + 1) in black_locations:
            moves_list.append((x - 1, y + 1))

    else:
        if (x, y - 1) not in white_locations and (x, y - 1) not in black_locations and y > 0:
            moves_list.append((x, y - 1))
        if (x, y - 2) not in white_locations and (x, y - 2) not in black_locations and y == 6:
            moves_list.append((x, y - 2))
        if (x + 1, y - 1) in white_locations:
            moves_list.append((x + 1, y - 1))
        if (x - 1, y - 1) in white_locations:
            moves_list.append((x - 1, y - 1))

    return moves_list
def valid_moves(piece, turn, location):
    moves_list = []
    all_moves_list = []
    if piece == 'pawn':
        moves_list = check_pawn(location, turn)
    #elif piece == 'queen':
    #    moves_list = check_queen(location, turn)
    #elif piece == 'king':
    #    moves_list = check_king(location, turn)
    #elif piece == 'knight':
    #    moves_list = check_knight(location, turn)
    #elif piece == 'rook':
    #    moves_list = check_rook(location, turn)
    #elif piece == 'bishop':
    #    moves_list = check_bishop(location, turn)
    print(moves_list)
    return moves_list

while run:
    pygame.time.Clock().tick(60)
    draw_board()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            x_coord = event.pos[0] // 100
            y_coord = event.pos[1] // 100
            click_coords = (x_coord, y_coord)
            print(click_coords, turn)
            possible_moves = []
            if turn <= 1:
                if click_coords in white_locations:
                    selection = white_locations.index(click_coords)
                    white_piece = white_pieces[selection]
                    print(white_piece)
                    possible_moves = valid_moves(white_piece, turn, click_coords)
                    print(possible_moves)
                    if turn == 0: turn = 1
                    if click_coords in black_locations and click_coords in possible_moves:
                        selection = black_locations.index(click_coords)
                        black_piece = black_pieces[selection]
                        black_pieces.pop(black_piece)
                        black_locations.pop(selection)
                        white_locations[selection] = click_coords
                        turn = 2
                        selection = 100

                    elif click_coords in possible_moves:
                        print("elif is working")
                        white_locations[selection] = click_coords
                        possible_moves = []
                        turn = 2

            if turn > 1:
                if click_coords in white_locations:
                    selection = black_locations.index(click_coords)
                    black_piece = black_pieces[selection]
                    print(black_piece)
                    possible_moves = valid_moves(black_piece, turn, click_coords)
                    if turn == 2: turn = 3
                    if click_coords in white_locations and click_coords in possible_moves:
                        selection = black_locations.index(click_coords)
                        white_piece = white_pieces[selection]
                        white_pieces.pop(white_piece)
                        white_locations.pop(selection)
                        black_locations[selection] = click_coords
                        turn = 2
                        selection = 100
                    elif click_coords in possible_moves:
                        black_locations[selection] = click_coords
                        possible_moves = []
                        turn = 3

    pygame.display.flip()
pygame.quit()