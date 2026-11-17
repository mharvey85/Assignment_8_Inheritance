#include <stdio.h>
#include <iostream>
#include <math.h>

// Physical constants
const double EPSILON_0 = 8.854187817e-12; // Permittivity of free space (F/m)
const double MU_0 = 4.0 * M_PI * 1e-7;    // Permeability of free space (T*m/A)

/*Base Class: Field*/
class field{
protected:
    double *value;

public:
    // Default Constructor
    field() { value = new double[3]{0.0, 0.0, 0.0}; }

    // Component constructor
    field(double x, double y, double z) { value = new double[3]{x, y, z}; }

    // Destructor
    virtual ~field() { delete[] value; }

    // Getters
    double getX() const { return value[0]; }
    double getY() const { return value[1]; }
    double getZ() const { return value[2]; }

    // Setters
    void setX(double x) { value[0] = x; }
    void setY(double y) { value[1] = y; }
    void setZ(double z) { value[2] = z; }

    // Function that calculates the magnitude of the vector
    double calculateMagnitude() {
        return std::sqrt( (value[0] * value[0]) + (value[1] * value[1]) + (value[2] * value[2]) );
    }

    // Function that prints out the magnitude of the vector
    void printMagnitude(){
        double mag = calculateMagnitude();
        if (mag == 0.0) {
            std::cout << "<0, 0, 0> (Magnitude is zero, unit vector undefined)";
        } else {
            std::cout << "<" << value[0] << ", " 
                      << value[1] << ", " 
                      << value[2] << ">" ;
        }

    } 

};

/*Derived Class: Electric Field*/
class Electric_Field: public field {private:
    double calculatedE;

public:
    // Default constructor
    Electric_Field() : field(), calculatedE(0.0) {}

    // Parameterized constructor calling base class constructor
    Electric_Field(double x, double y, double z) : field(x, y, z), calculatedE(0.0) {}

    // Copy constructor
    Electric_Field(const Electric_Field &other) : field(other), calculatedE(other.calculatedE) {}

    // Copy assignment operator
    Electric_Field& operator=(const Electric_Field &other) {
        if (this != &other) {
            field::operator=(other);
            calculatedE = other.calculatedE;
        }
        return *this;
    }

    // Gauss' Law for a spherical charge: E = Q / (4 * pi * r^2 * epsilon_0)
    void calculateElectricField(double Q, double r) {
        if (r == 0.0) {
            std::cerr << "Error: Distance r cannot be zero." << std::endl;
            return;
        }
        calculatedE = Q / (4.0 * M_PI * r * r * EPSILON_0);
    }

    double getCalculatedE() const {
        return calculatedE;
    }

    // Overload '+' operator
    Electric_Field operator+(const Electric_Field &other) const {
        return Electric_Field(
            value[0] + other.value[0],
            value[1] + other.value[1],
            value[2] + other.value[2]
        );
    }

    // Overload '<<' operator
    friend std::ostream& operator<<(std::ostream &os, const Electric_Field &ef) {
        os << "Electric Field Components: (" << ef.value[0] << ", " << ef.value[1] << ", " << ef.value[2] << ")";
        return os;
    }
};

/*Derived Class: Magnetic Field*/
class Magnetic_Field : public field {
private:
    double calculatedB;

public:
    // Default constructor
    Magnetic_Field() : field(), calculatedB(0.0) {}

    // Parameterized constructor calling base class constructor
    Magnetic_Field(double x, double y, double z) : field(x, y, z), calculatedB(0.0) {}

    // Copy constructor
    Magnetic_Field(const Magnetic_Field &other) : field(other), calculatedB(other.calculatedB) {}

    // Copy assignment operator
    Magnetic_Field& operator=(const Magnetic_Field &other) {
        if (this != &other) {
            field::operator=(other);
            calculatedB = other.calculatedB;
        }
        return *this;
    }

    // Ampere's Law for a current-carrying wire: B = (mu_0 * I) / (2 * pi * r)
    void calculateMagneticField(double I, double r) {
        if (r == 0.0) {
            std::cerr << "Error: Distance r cannot be zero." << std::endl;
            return;
        }
        calculatedB = (MU_0 * I) / (2.0 * M_PI * r);
    }

    double getCalculatedB() const {
        return calculatedB;
    }

    // Overload '+' operator
    Magnetic_Field operator+(const Magnetic_Field &other) const {
        return Magnetic_Field(
            value[0] + other.value[0],
            value[1] + other.value[1],
            value[2] + other.value[2]
        );
    }

    // Overload '<<' operator
    friend std::ostream& operator<<(std::ostream &os, const Magnetic_Field &mf) {
        os << "Magnetic Field Components: (" << mf.value[0] << ", " << mf.value[1] << ", " << mf.value[2] << ")";
        return os;
    }
};

int main() {
    // 1. Create Electric and Magnetic Field objects with parameterized constructors
    Electric_Field e_field(0.0, 1e5, 1e3);
    Magnetic_Field b_field(0.5, 2.3, 4.1);

    // 2. Use printMagnitude() from the base class
    std::cout << "--- Initial Components (printMagnitude) ---" << std::endl;
    std::cout << "Electric Field: ";
    e_field.printMagnitude();
    std::cout << std::endl;

    std::cout << "Magnetic Field: ";
    b_field.printMagnitude();
    std::cout << "\n" << std::endl;

    // 3. Calculate fields via Gauss' Law and Ampere's Law
    double Q = 5e-6; // 5 microCoulombs
    double r_e = 0.05; // 5 cm
    e_field.calculateElectricField(Q, r_e);

    double I = 10.0; // 10 Amps
    double r_b = 0.02; // 2 cm
    b_field.calculateMagneticField(I, r_b);

    std::cout << "--- Physics Law Calculations ---" << std::endl;
    std::cout << "Calculated Electric Field (Q = 5uC, r = 0.05m): " << e_field.getCalculatedE() << " N/C" << std::endl;
    std::cout << "Calculated Magnetic Field (I = 10A, r = 0.02m): " << b_field.getCalculatedB() << " T" << std::endl;
    std::cout << std::endl;

    // 4. Demonstrate '+' and '<<' overloaded operators
    std::cout << "--- Operator Overloading (+ and <<) ---" << std::endl;
    Electric_Field e1;
    Electric_Field e2(100.0, 200.0, 300.0);
    Electric_Field e3(50.0, 50.0, 50.0);
    e1 = e2 + e3;
    std::cout << e1 << std::endl;

    Magnetic_Field b1;
    Magnetic_Field b2(1.2, 3.4, 5.6);
    Magnetic_Field b3(0.8, 0.6, 0.4);
    b1 = b2 + b3;
    std::cout << b1 << std::endl;

    return 0;
}