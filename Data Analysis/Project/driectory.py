from dataclasses import dataclass
from pathlib import Path


@dataclass
class Directory:  # 나중에 파일 이름 고쳐서 쓰기
    files: Path = Path('txts')
    images: Path = Path('images')
    map_data: Path = Path('map_data.txt')
    map: Path = Path('map.png')
    winds: Path = Path('wind.txt')
    dusts: Path = Path('dust.txt')
    dust_map: Path = Path('GK2_GEMS_L4_20231101_0245_PM25-SURFACE_ORI_KR.png')
    dust_image: Path = Path(
        '2_GK2_GEMS_L4_20231101_0245_PM25-SURFACE_ORI_KR.png')
