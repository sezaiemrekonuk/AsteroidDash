#include "CelestialObject.h"
#include <iostream>

// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance)
{
    right_rotation = nullptr;
    left_rotation = nullptr;
    next_celestial_object = nullptr;
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
    : shape(other->shape),                          // Copy the 2D vector shape
      object_type(other->object_type),              // Copy the object type
      starting_row(other->starting_row),            // Copy the starting row
      time_of_appearance(other->time_of_appearance) // Copy the time of appearance
{
    // TODO: Your code here
    if (other->right_rotation)
    {
        right_rotation = new CelestialObject(other->right_rotation);
    }
    if (other->left_rotation)
    {
        left_rotation = new CelestialObject(other->left_rotation);
    }
    if (other->next_celestial_object)
    {
        next_celestial_object = new CelestialObject(other->next_celestial_object);
    }
}

// Generate rotations for the current celestial object. 90-degree, 180-degree, and 270-degree.
void CelestialObject::generate_rotations()
{
    CelestialObject *current = this;
    std::vector<std::vector<bool>> original_shape = this->shape;

    // 90
    std::vector<std::vector<bool>> rotated_shape = rotate_clockwise(current->shape);
    CelestialObject *rotation = new CelestialObject(rotated_shape, object_type, starting_row, time_of_appearance);

    // 180 if equal to original, connect to 90-degree rotation only
    std::vector<std::vector<bool>> rotation_180 = rotate_clockwise(rotated_shape);
    if (rotation_180 == original_shape)
    {
        // Connect original to 90-degree rotation only
        this->right_rotation = rotation;
        rotation->left_rotation = this;
        rotation->right_rotation = this;
        this->left_rotation = rotation;
        return;
    }

    // if not equeal continue with others
    current->right_rotation = rotation;
    rotation->left_rotation = current;
    current = rotation;

    for (int i = 1; i < 3; ++i)
    {
        rotated_shape = rotate_clockwise(current->shape);
        rotation = new CelestialObject(rotated_shape, object_type, starting_row, time_of_appearance);
        current->right_rotation = rotation;
        rotation->left_rotation = current;
        current = rotation;
    }

    // complete the circular list
    current->right_rotation = this;
    this->left_rotation = current;
}

std::vector<std::vector<bool>> CelestialObject::rotate_clockwise(const std::vector<std::vector<bool>> &matrix)
{
    if (matrix.empty() || matrix[0].empty())
    {
        return matrix;
    }

    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    std::vector<std::vector<bool>> rotated(cols, std::vector<bool>(rows, false));

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            rotated[j][rows - 1 - i] = matrix[i][j];
        }
    }
    return rotated;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target)
{
    if (target == nullptr)
        return;

    CelestialObject *current = target->right_rotation;
    target->right_rotation = nullptr;

    while (current != nullptr && current != target)
    {
        CelestialObject *next = current->right_rotation;
        delete current;
        current = next;
    }

    target = nullptr;
}

// Destructor to free the dynamically allocated memory for the object's shape
CelestialObject::~CelestialObject()
{
    shape.clear();
}