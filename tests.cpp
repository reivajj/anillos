#include "anillo.h"

// auxiliares para tests
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;
void mt_assert(bool expr, int line) {
	if (!expr) {
		ostringstream os;
		os << "error en linea " << line;
		throw os.str();
	}
}
#define RUN_TEST(test) {\
	{bool ok = true;\
	cout << #test << "..." << flush;\
	try { test(); }\
	catch (string& msg) { ok = false; cout << msg; } \
	catch (...) { ok = false; cout << "error"; }\
	if (ok) { cout << "ok"; }\
	cout << endl << flush;\
	}\
}
#define ASSERT_EQ(lhs, rhs) { mt_assert((lhs) == (rhs), __LINE__); }
#define ASSERT(expr) { mt_assert((expr), __LINE__); }
#define ASSERT_STR_EQ(lhs, rhs) { mt_assert(string((lhs)).compare((rhs)) == 0, __LINE__); }
// fin auxiliares para tests

template<typename T>
string to_s(const Anillo<T>* a) {
	ostringstream os;
	os << *a;
	return os.str();
}

void AnilloNuevoEsVacio() {
	Anillo<int>* a = new Anillo<int>();
	ASSERT(a->esVacio());
	ASSERT_EQ(a->tamanio(), 0);
	delete a;
}

void AnilloUnitarioDaSiguiente() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	ASSERT(!a->esVacio());
	ASSERT_EQ(a->tamanio(), 1);
	ASSERT_EQ(a->siguiente(), 42);
	delete a;
}

void MostrarAnilloVacio() {
	Anillo<int>* a = new Anillo<int>();
	ASSERT_STR_EQ(to_s(a), "[]");
	delete a;
}

void AnilloPuedeRotarVariasVeces() {

}

// borrar
void AnilloUnitarioCicla() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	ASSERT_EQ(a->siguiente(), 42);
	ASSERT_EQ(a->siguiente(), 42);
	ASSERT_EQ(a->siguiente(), 42);
	ASSERT_EQ(a->siguiente(), 42);
	delete a;
}

void UltimoEnAgregarEsElSiguiente() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	ASSERT_EQ(a->siguiente(), 42);
	a->agregar(20);
	ASSERT_EQ(a->siguiente(), 20);
	delete a;
}

void AnilloConDosCicla() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	a->agregar(20);
	ASSERT_EQ(a->siguiente(), 20);
	ASSERT_EQ(a->siguiente(), 42);
	ASSERT_EQ(a->siguiente(), 20);
	ASSERT_EQ(a->siguiente(), 42);
	delete a;
}

void AnilloSacaNoSiguiente() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	a->agregar(20);
	a->eliminar(42);
	ASSERT_EQ(a->siguiente(), 20);
	ASSERT_EQ(a->tamanio(), 1);
	delete a;
}

void MostrarAnillo() {
	Anillo<int>* a = new Anillo<int>();
	ASSERT_STR_EQ(to_s(a), "[]");
	a->agregar(42);
	ASSERT_STR_EQ(to_s(a), "[42]");
	a->agregar(2);
	ASSERT_STR_EQ(to_s(a), "[2, 42]");
	a->siguiente();
	ASSERT_STR_EQ(to_s(a), "[42, 2*]");
	a->siguiente();
	ASSERT_STR_EQ(to_s(a), "[2, 42*]");
	delete a;
}

void AnillosClonadoPreservaAnterior() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(42);
	a->agregar(20);
	a->siguiente();
	Anillo<int>* a2 = new Anillo<int>(*a);
	ASSERT_STR_EQ(to_s(a), "[42, 20*]");
	ASSERT_STR_EQ(to_s(a2), "[42, 20*]");
	delete a;
    delete a2;
}

void PuedeEliminarAnterior() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(10);
	a->agregar(20);
	a->agregar(30);
	a->siguiente();
	ASSERT_STR_EQ(to_s(a), "[20, 10, 30*]");
	ASSERT(a->huboAnterior());
	a->eliminar(30);
	ASSERT_STR_EQ(to_s(a), "[20, 10]");
	ASSERT(!a->huboAnterior());
	delete a;
}

void RetrocederUnaVezRepite() {
	Anillo<int>* a = new Anillo<int>();
	a->agregar(10);
	a->agregar(20);
	a->agregar(30);
	a->siguiente();
	ASSERT_STR_EQ(to_s(a), "[20, 10, 30*]");
	a->retroceder();
	ASSERT_STR_EQ(to_s(a), "[30*, 20, 10]");
	a->retroceder();
	ASSERT_STR_EQ(to_s(a), "[10, 30*, 20]");
	delete a;
}

void AnilloDeAnillo(){
	Anillo<Anillo<int> > a;
	a.agregar(Anillo<int>());
	Anillo<Anillo<int> > b(a);
}

// fin borrar

int main(void) {
	RUN_TEST(AnilloNuevoEsVacio);
	RUN_TEST(AnilloUnitarioDaSiguiente);
	RUN_TEST(MostrarAnilloVacio);
	RUN_TEST(AnilloUnitarioCicla);
	RUN_TEST(UltimoEnAgregarEsElSiguiente);
	RUN_TEST(AnilloConDosCicla);
	RUN_TEST(AnilloSacaNoSiguiente);
	RUN_TEST(MostrarAnillo);
	RUN_TEST(AnillosClonadoPreservaAnterior);
	RUN_TEST(PuedeEliminarAnterior);
	RUN_TEST(RetrocederUnaVezRepite);
	RUN_TEST(AnilloDeAnillo);
	return 0;
}
