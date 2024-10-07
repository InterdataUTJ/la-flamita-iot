# Como compilar la herramienta de configuración 🛠️

Esta es la guía para poder compilar la herramienta de configuración para tu sistema, antes de comenzar es necesario tener algunos requisitos mínimos cubiertos.

## Prerrequisitos

1. [NodeJS](https://nodejs.org/) (_Te recomiendo la versión actual LTS 20.18.0_).
2. npm (_Instalado por defecto con NodeJS_).

## Guía paso a paso

### Paso 1. Clonar el repositorio de [GitHub](https://github.com/InterdataUTJ/la-flamita-iot/) y navegar al directorio.

```bash
git clone https://github.com/InterdataUTJ/la-flamita-iot.git
cd la-flamita-iot/iot-config
```

### Paso 2. Instalar las dependencias del proyecto.

```bash
npm install
```

### Paso 3. Ejecutar el comando de [compilación](https://www.electron.build/index.html#quick-setup-guide).

Para este paso es necesario tomar en cuenta en que plataforma queremos ejecutar la herramienta, el [`package.json`]() está configurado con las siguientes opciones:

#### Windows

- **`build:win:64`** Para ejecutarse en `Windows` de `64 bits` de forma `portable`.
- **`build:win:32`** Para ejecutarse en `Windows` de `32 bits` de forma `portable`. 
- **`build:win:armv71`** Para ejecutarse en `Windows ARM` de `32 bits` de forma `portable`.
- **`build:win:arm64`** Para ejecutarse en `Windows ARM` de `64 bits` de forma `portable`. 

#### Linux

- **`build:linux:64`** Para ejecutarse en `Linux` de `64 bits` usando un `AppImage`.
- **`build:linux:32`** Para ejecutarse en `Linux` de `32 bits` usando un `AppImage`. 
- **`build:linux:armv71`** Para ejecutarse en `Linux ARM` de `32 bits` usando un `AppImage`.
- **`build:linux:arm64`** Para ejecutarse en `Linux ARM` de `64 bits` usando un `AppImage`. 

#### MacOS

- **`build:mac:64`** Para ejecutarse en `MacOS` de `64 bits`.
- **`build:mac:32`** Para ejecutarse en `MacOS` de `32 bits`. 
- **`build:mac:armv71`** Para ejecutarse en `MacOS ARM` de `32 bits`.
- **`build:mac:arm64`** Para ejecutarse en `MacOS ARM` de `64 bits`. 
- **`build:mac:arm64`** Para ejecutarse en `MacOS ARM` de `64 bits`. 
- **`build:mac:universal`** Para ejecutarse en `MacOS` con un [`Universal Binary`](https://developer.apple.com/documentation/apple-silicon/building-a-universal-macos-binary). 

```bash
# Remplazar el build:win:64 por la opción que necesites
npm run build:win:64
```

### Paso 4. Ejecutar el binario

Una vez terminado el proceso ya deberías de encontrar tu ejecutable listo dentro de la carpeta `/dist`. Para `linux` encontrarás un archivo `AppImage`, mientras que para Windows habrá un `exe` portable. El proceso de compilación no ha sido probado en `macos` y solamente se documentó siguiendo la información oficial de [`electron-builder`](https://www.electron.build/index.html). 

```bash
# Ejemplo para ejecutar en Windows 64 bits desde la terminal 
./dist/la-flamita-iot-1.0.1-win-x64.exe
```