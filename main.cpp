#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QString>
#include <QMessageBox> // Librería para alertas profesionales emergentes
#include <string>
#include <fstream>

class Insumo {
public:
    int id;
    std::string nombre;
    float cantidad;
    std::string medida;
};

class VentanaInventario : public QWidget {
public:
    // Cajas para Registrar Entradas
    QLineEdit *txtId;
    QLineEdit *txtNombre;
    QLineEdit *txtCantidad;
    QLineEdit *txtMedida;

    // Cajas para Retirar Salidas
    QLineEdit *txtIdSalida;
    QLineEdit *txtCantSalida;

    QListWidget *listaVisual;
    
    Insumo inventario[100];
    int total_insumos = 0;

    VentanaInventario() {
        setWindowTitle("Almacén SIGSA - Sistema de Gestión");
        resize(450, 700); 

        QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

        // --- 1. ZONA DE REGISTRO (ENTRADAS) ---
        layoutPrincipal->addWidget(new QLabel("<b>NUEVO INGRESO:</b>"));
        QFormLayout *formRegistro = new QFormLayout();
        txtId = new QLineEdit();
        txtNombre = new QLineEdit();
        txtCantidad = new QLineEdit();
        txtMedida = new QLineEdit();
        
        formRegistro->addRow("ID del Producto:", txtId);
        formRegistro->addRow("Nombre (sin espacios):", txtNombre);
        formRegistro->addRow("Cantidad:", txtCantidad);
        formRegistro->addRow("Unidad (KG, Litros, Pz):", txtMedida);
        
        QPushButton *btnRegistrar = new QPushButton("Registrar Insumo");
        layoutPrincipal->addLayout(formRegistro);
        layoutPrincipal->addWidget(btnRegistrar);

        // --- 2. ZONA DE SALIDAS (COCINA) ---
        layoutPrincipal->addWidget(new QLabel("<b>SALIDA DE INVENTARIO:</b>"));
        QFormLayout *formSalida = new QFormLayout();
        txtIdSalida = new QLineEdit();
        txtCantSalida = new QLineEdit();
        
        formSalida->addRow("ID a retirar:", txtIdSalida);
        formSalida->addRow("Cantidad a sacar:", txtCantSalida);
        
        QPushButton *btnDescontar = new QPushButton("Descontar Inventario");
        layoutPrincipal->addLayout(formSalida);
        layoutPrincipal->addWidget(btnDescontar);

        // --- 3. ZONA DE VISUALIZACIÓN ---
        layoutPrincipal->addWidget(new QLabel("<b>INVENTARIO ACTUAL:</b>"));
        listaVisual = new QListWidget();
        layoutPrincipal->addWidget(listaVisual);

        // --- CARGA INICIAL ---
        std::ifstream archivo_lectura("inventario.txt");
        if (archivo_lectura.is_open()) {
            int id_leido;
            std::string nombre_leido;
            float cant_leida;
            std::string medida_leida;

            while (archivo_lectura >> id_leido >> nombre_leido >> cant_leida >> medida_leida) {
                inventario[total_insumos].id = id_leido;
                inventario[total_insumos].nombre = nombre_leido;
                inventario[total_insumos].cantidad = cant_leida;
                inventario[total_insumos].medida = medida_leida;
                total_insumos++;
            }
            archivo_lectura.close();
        }
        refrescarPantallaYArchivo(); // Pinta la lista al abrir el programa

        // --- CONEXIONES ---
        connect(btnRegistrar, &QPushButton::clicked, this, &VentanaInventario::guardarProducto);
        connect(btnDescontar, &QPushButton::clicked, this, &VentanaInventario::descontarProducto);
    }

    // Función auxiliar: Sobrescribe el TXT y redibuja la lista blanca
    void refrescarPantallaYArchivo() {
        std::ofstream archivo("inventario.txt"); 
        if (archivo.is_open()) {
            for(int i = 0; i < total_insumos; i++) {
                archivo << inventario[i].id << " " 
                        << inventario[i].nombre << " " 
                        << inventario[i].cantidad << " " 
                        << inventario[i].medida << std::endl;
            }
            archivo.close();
        }

        listaVisual->clear();
        for(int i = 0; i < total_insumos; i++) {
            QString linea = QString::number(inventario[i].id) + " - " + 
                            QString::fromStdString(inventario[i].nombre) + 
                            " (" + QString::number(inventario[i].cantidad) + " " + 
                            QString::fromStdString(inventario[i].medida) + ")";
            listaVisual->addItem(linea);
        }
    }

    void guardarProducto() {
        inventario[total_insumos].id = txtId->text().toInt();
        inventario[total_insumos].nombre = txtNombre->text().toStdString();
        inventario[total_insumos].cantidad = txtCantidad->text().toFloat();
        inventario[total_insumos].medida = txtMedida->text().toStdString();
        total_insumos++;

        refrescarPantallaYArchivo();

        txtId->clear();
        txtNombre->clear();
        txtCantidad->clear();
        txtMedida->clear();
        txtId->setFocus(); 
    }

    void descontarProducto() {
        int id_buscado = txtIdSalida->text().toInt();
        float cant_retirar = txtCantSalida->text().toFloat();
        bool encontrado = false;

        for(int i = 0; i < total_insumos; i++) {
            if(inventario[i].id == id_buscado) {
                encontrado = true;
                if(cant_retirar <= inventario[i].cantidad && cant_retirar > 0) {
                    inventario[i].cantidad -= cant_retirar; // Resta matemática
                    refrescarPantallaYArchivo(); // Refleja el cambio al instante
                    QMessageBox::information(this, "Éxito", "Retiro registrado correctamente.");
                } else {
                    QMessageBox::warning(this, "Error", "Cantidad inválida o superior al stock disponible.");
                }
                break;
            }
        }

        if(!encontrado) {
            QMessageBox::critical(this, "Error", "El ID ingresado no existe en el sistema.");
        }

        txtIdSalida->clear();
        txtCantSalida->clear();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    VentanaInventario ventana;
    ventana.show();
    return app.exec();
}