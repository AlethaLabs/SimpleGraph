/*  
    The goal of this project is to:
        - Display a basic window with a graph
        - Draw points to the graph by user clicking
        - Make necessary calculations to determine and display a simple regression line
        - Use the 'least squares algorithm' to display line with multiple points

    This program is meant to be extremely simple and explore some concepts in C but
    I would like to have some future addatives such as:
        - Better looking UI
        - Ability to upload a csv file
        - Multivariable regression
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <raylib.h>

/* 
Solve for y = mx + b

m = slope of the line
b = y intercept
x = x coordinate at any point of the line
y = y coordinate at any point of the line

From the two plotted points I will get x1, x2, y1, y2
- m = y2-y1/x2-x1 (rise over run)
- from the array above it should be [x1, y1, x2, y2]
*/
typedef struct {
    float m;
    float b;
} RegressionLine;

RegressionLine simple_regression(Vector2 p1, Vector2 p2) {
    float x1 = p1.x;
    float y1 = p1.y;
    float x2 = p2.x;
    float y2 = p2.y;
    
    if (x2 == x1) {
        printf("Error: Slope undefined\n");
        return (RegressionLine){0.0f, 0.0f};
    }
    
    float m = (y2 - y1) / (x2 - x1);
    float b = y1 - m * x1;
    
    return (RegressionLine){m, b};
}

typedef struct {
    float y_mean;
    float x_mean;
    float d_sum;
    float xsd_sum;
    float m;
    float b;
    int valid;
} MultiLine;

/*
    To calculate this I need to use the least squares algorithm:
        - Calculate means for x and y
        - Callculate deviations
        - Sum the deviations
        - Sum the x-squared deviation
        - Sum of deviations/x-squared deviation - get m
        - Calculate intercept b = y-mean - m * x-mean
        - y = mx + b
*/
MultiLine multi_regression(Vector2 *points, int count) {
    float x_sum = 0.0f;
    float y_sum = 0.0f;
    float d_sum = 0.0f;
    float xsd_sum = 0.0f;
    float m = 0.0f;
    float b = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    if (count < 2) {
        return (MultiLine){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0};
    }
    for (int i = 0; i < count; i++) {
        x_sum += points[i].x;
        y_sum += points[i].y;
    }
    float x_mean = x_sum/count;
    float y_mean = y_sum/count;

    for (int j = 0; j < count; j++) {
        dx = points[j].x - x_mean;
        dy = points[j].y - y_mean; 
        d_sum += dx * dy;
        xsd_sum += dx * dx;
    } 

    if (fabsf(xsd_sum) < FLT_EPSILON) {
        printf("Error: Sum of x-squared deviations returned as 0\n");
        return (MultiLine){0.0f, 0.0f, d_sum, xsd_sum, 0.0f, 0.0f, 0};
    }

    m = d_sum / xsd_sum;
    b = y_mean - m * x_mean;

    return (MultiLine){y_mean, x_mean, d_sum, xsd_sum, m, b, 1};
}

// Get mouse coordinates, store button clicks for x, y in a dynamic array, draw points
void draw_points(Vector2 *points, int *count, int max_size) {
    if (points == NULL) { printf("Failed to allocate array..."); exit(0); }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        if (*count < max_size) {
            points[*count] = pos;
            (*count)++;
        }
    }

    for (int i = 0; i < *count; i++) {
        DrawCircle(points[i].x, points[i].y, 5.0, RED);
    }
}

void window() {
    // Initiate array of x, y coordinates
    int max_size = 30;
    int count = 0;
    Vector2 *points = (Vector2 *)malloc(max_size * sizeof(Vector2));
    if (points == NULL) { printf("Failed to allocate memory for x, y coordinates"); exit(0); }

    // Draw window, create loop while window is not closed
    InitWindow(900, 900, "Graph");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Horizontal lines
        for (int y = 0; y < 900; y += 30) {
            DrawLine(y, 0, y, 900, GRAY);
        }
        // Vertical lines
        for (int x = 0; x < 900; x += 30) {
            DrawLine(0, x, 900, x, GRAY);
        }
        draw_points(points, &count, max_size);
        if (count == 2) {
            RegressionLine line = simple_regression(points[0], points[1]);
            int xLeft = 0;
            int xRight= 900;
            int yLeft = line.m * xLeft + line.b;
            int yRight = line.m * xRight + line.b;
            DrawLine(xLeft, yLeft, xRight, yRight, GREEN);
        }
        if (count > 2) {
            MultiLine m_line = multi_regression(points, count);
            if (m_line.valid) {
                int xLeft =0;
                int xRight = 900;
                int yLeft = m_line.m * xLeft + m_line.b;
                int yRight = m_line.m * xRight + m_line.b;
                DrawLine(xLeft, yLeft, xRight, yRight, GREEN);
            }
        }
        EndDrawing();
    }
    CloseWindow();
    free(points);
}

int main() {
    window();
    return 0;
}