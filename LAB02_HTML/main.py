import os
import threading
from html.parser import HTMLParser
from urllib.parse import urlparse


class LocalFileDownloader:
    def __init__(self, output_directory):
        self.output_directory = output_directory
        os.makedirs(output_directory, exist_ok=True)
        print('caralho')

    def download_file(self, url):
        try:
            parsed_url = urlparse(url)
            filename = os.path.join(self.output_directory, os.path.basename(parsed_url.path))
            with open(filename, "wb") as file:
                file.write(open(parsed_url.path, "rb").read())
        except Exception as e:
        #     print(f"Error when downloading {url}: {e}")
            pass


class LocalFileHTMLParser(HTMLParser):
    def __init__(self, downloader):
        super().__init__()
        self.downloader = downloader
        self.links = set()

    def handle_starttag(self, tag, attrs):
        if tag in ("link", "img", "script"):
            for attr, value in attrs:
                if attr in ("href", "src"):
                    self.links.add(value)


def main():
    html_file = "C:/Users/unifgalima/Downloads/Sistemas Distribuidos/Sistemas Distribuidos/Exemplo/pagina.html"  # rename by file name in HTML
    output_directory = "output_directory"

    downloader = LocalFileDownloader(output_directory)
    parser = LocalFileHTMLParser(downloader)

    with open(html_file, "r", encoding="utf-8") as file:
        html_content = file.read()

    parser.feed(html_content) 
    links = parser.links

    threads = []
    for link in links:
        thread = threading.Thread(target=downloader.download_file, args=(link,))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    print(f"Files downloaded from {output_directory}")


if __name__ == "__main__":
    main()